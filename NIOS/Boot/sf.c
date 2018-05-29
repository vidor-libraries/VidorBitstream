/*
 * sf.c
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#include <string.h>
#include <system.h>
#include <altera_avalon_spi.h>

#define SF_FAST_RD
#define SF_SECURITY

/**
 *
 */
alt_u32 sfJedecId(void);
alt_u32 sfUniqueId(alt_u8* id);
alt_u32 sfErase(alt_u32 mode, alt_u32 adr);
alt_u32 sfProgram(alt_u32 adr, alt_u8* data, alt_u32 len);
alt_u32 sfRead(alt_u32 adr, alt_u8* data, alt_u32 len);

#ifdef SF_SECURITY
/**
 * Security Register
 */
alt_u32 sfSRErase(alt_u8 reg);
alt_u32 sfSRProgram(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);
alt_u32 sfSRLock(alt_u8 reg);
alt_u32 sfSRRead(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);

alt_u32 sfProtect(void);
#endif  /* SF_SECURITY */


/**
 *
 */
#define SF_WRFLG_ERASE    0x00000001
#define SF_WRFLG_PROGRAM  0x00000002
#define SF_WRFLG_VERIFY   0x00000004
alt_u32 sfWrite(alt_u8* data, alt_u32 len, alt_u32 flg);

/**
 */
alt_u32 sfJedecId(void)
{
	alt_u8  txb[]={0x9f};
	alt_u8  rxb[4];

	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 3, rxb, 0);

	return (rxb[0]<<16) | (rxb[1]<<8) | (rxb[2]);
}

/**
 *
 */
alt_u32 sfUniqueId(alt_u8* id)
{
	alt_u8  txb[1+4];

	txb[0] = 0x4B;
	txb[1] = 0;
	txb[2] = 0;
	txb[3] = 0;
	txb[4] = 0;

	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1+4, txb, 8, id, 0);

	return 0;
}

/**
 * While the device is executing a successful erase cycle, the Status Register
 * can be read and will indicate that the device is busy.
 * For faster throughput, it is recommended that the Status Register be polled
 * rather than waiting the tCHPE  time to determine if the device has finished
 * erasing. At some point before the erase cycle completes, the WEL bit in the
 * Status Register will be reset back to the logical “0” state.
 *
 * tCHPE Chip Erase Time Typ: 12 sec, Max: 20 sec, 30 sec
 *
 */
alt_u32 sfErase(alt_u32 mode, alt_u32 adr)
{
	alt_u8 txb[4];
	alt_u8 txl;
	alt_u8 rxb[1];

	//write enable 0x06
	txb[0] = 0x06;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 0, rxb, 0);

	switch(mode){
	case 0:
		// Sector Erase   	 4K sector address  0x020
		adr >>= 12;
		txb[0] = 0x020;
		txb[1] = adr>>16;
		txb[2] = adr>>8;
		txb[3] = adr;
		txl = 4;
		break;
	case 1:
		// 32KB Block Erase	32K Block address   0x52
		adr >>= 15;
		txb[0] = 0x52;
		txb[1] = adr>>16;
		txb[2] = adr>>8;
		txb[3] = adr;
		txl = 4;
		break;
	case 2:
		// 64KB Block Erase 64K Block address    0xD8
		adr >>= 16;
		txb[0] = 0xD8;
		txb[1] = adr>>16;
		txb[2] = adr>>8;
		txb[3] = adr;
		txl = 4;
		break;
	case 3:
		// Chip Erase       Not used            0x60 or 0xC7
		txb[0] = 0xC7;
		txl = 1;
		break;
	default:
		return -1;
	}
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, txl, txb, 0, rxb, 0);

	// wait tCHPE 12/20/30 sec
	txb[0] = 0x05;
	do {alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);} while (rxb[0]&1);

	return 0;
}

/**
 */
alt_u32 sfProgram(alt_u32 adr, alt_u8* data, alt_u32 len)
{
	alt_u8  txb[1+3+256];
	alt_u8  rxb[1];
	alt_u32 ptr;
	alt_u32 cnt;

	ptr = 0;
	do{
		//write enable 0x06
		txb[0] = 0x06;
		alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 0, rxb, 0);

		//program      0x02 address (3 byte) data (1-256)
		//max data length is 256 byte
		cnt = (len+(adr & 0xFF))>256? (256-(adr & 0xFF)): len;

		txb[0] = 0x02;
		txb[1] = adr>>16;
		txb[2] = adr>>8;
		txb[3] = adr;
#if 1
		memcpy((char*)txb+4, (char*)data+ptr, cnt);
#else
		memcpyr((char*)txb+4, (char*)data+ptr, cnt);
#endif
		alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1+3+cnt, txb, 0, rxb, 0);
		len -= cnt;
		adr += cnt;
		ptr += cnt;

		txb[0] = 0x05;
		do { alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);} while (rxb[0]&1);
	}while(len>0);

	return 0;
}

/**
 * Two opcodes (0Bh and 03h) can be used for the Read Array command.
 * The use of each opcode depends on the maximum clock frequency
 * that will be used to read data from the device.
 * The 0Bh opcode can be used at any clock frequency up to the
 * maximum specified by fRDHF,
 * and the 03h opcode can be used for lower frequency read
 * operations up to the maximum specified by fRDLF.
 *
 */
alt_u32 sfRead(alt_u32 adr, alt_u8* data, alt_u32 len)
{
	alt_u8  txb[1+3+1];
	alt_u32 ptr;
	alt_u32 cnt;

	ptr = 0;
	do{
		//max data length is 256 byte
		cnt = (len+(adr & 0xFF))>256? (256-(adr & 0xFF)): len;

#ifdef SF_FAST_RD
		txb[0] = 0x0B;
		txb[4] = 0x00;
#else
		txb[0] = 0x03;
#endif
		txb[1] = adr>>16;
		txb[2] = adr>>8;
		txb[3] = adr;

#ifdef SF_FAST_RD
		alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1+3+1, txb, cnt, data+ptr, 0);
#else
		alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1+3, txb, cnt, data+ptr, 0);
#endif

		len -= cnt;
		adr += cnt;
		ptr += cnt;
	}while(len>0);

	return 0;
}

#ifdef SF_SECURITY
/**
 * @param reg Security Register index 0-2
 */
alt_u32 sfSRErase(alt_u8 reg)
{
	if (reg>2) {
		return -1;
	}
	alt_u8  txb[1+3];
	alt_u8  rxb[1];

	//write enable 0x06
	txb[0] = 0x06;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 0, rxb, 0);

	//Erase Security Registers (44h)
	txb[0] = 0x44;
	txb[1] = 0;
	txb[2] = (reg+1)<<4;
	txb[3] = 0;

	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1+3, txb, 0, rxb, 0);

	txb[0] = 0x05;
	do { alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);} while (rxb[0]&1);

	return 0;
}

/**
 * @param reg Security Register index 0-2
 * @param adr
 * @param data
 * @param len
 */
alt_u32 sfSRProgram(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len)
{
	if (reg>2) {
		return -1;
	}
	alt_u8  txb[1+3+256];
	alt_u8  rxb[1];

	//write enable 0x06
	txb[0] = 0x06;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 0, rxb, 0);

	//Program Security Registers
	txb[0] = 0x42;
	txb[1] = 0;
	txb[2] = (reg+1)<<4;
	txb[3] = adr;
	memcpy((char*)txb+4, (char*)data, len);

	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1+3+len, txb, 0, rxb, 0);

	txb[0] = 0x05;
	do { alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);} while (rxb[0]&1);

	return 0;
}

/**
 * The Security Register Lock Bits LB[3:1] in the Status Register-2
 * can be used to OTP protect the security registers.
 * Once a lock bit is set to 1, the corresponding  security  register
 * will  be  permanently locked, Program Security Register instruction
 * to that register will be ignored.
 *
 * Security Register Lock Bits (LB3, LB2, LB1)
 * The Security Register Lock Bits (LB3, LB2, LB1) are
 * non-volatile One Time Program (OTP) bits in Status
 * Register (S13, S12, S11) that provide the write protect
 * control and status to the Security Registers.
 * The default state of LB[3:1] is 0, Security Registers are unlocked.
 * LB[3:1] can be set to 1 individually using the Write Status Register instruction.
 * LB[3:1] are One Time Programmable (OTP), once it’s set to 1, the
 * corresponding 256-Byte Security Register will become read-only permanently.
 *
 */
alt_u32 sfSRLock(alt_u8 reg)
{
	if (reg>2) {
		return -1;
	}
	alt_u8  txb[1+3+2];
	alt_u8  rxb[1];
	alt_u8  sts1;
	alt_u8  sts2;

	// Read Status Register 1
	txb[0] = 0x05;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);
	sts1 = rxb[0];

	// Read Status Register 2
	txb[0] = 0x35;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);
	sts2 = rxb[0];

	// write enable 0x06
	txb[0] = 0x06;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 0, rxb, 0);

	// set LB bit
	sts2 |= 0x08 << reg;

	// Write Status Register 1 and 2 (0x01)
	txb[0] = 0x01;
	txb[1] = sts1;
	txb[2] = sts2;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 3, txb, 0, rxb, 0);

	txb[0] = 0x05;
	do { alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);} while (rxb[0]&1);

	return 0;
}

/**
 *
 */
alt_u32 sfSRRead(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len)
{
	if (reg>2) {
		return -1;
	}
	alt_u8  txb[1+4];

	//Read Security Registers (48h)
	txb[0] = 0x48;
	txb[1] = 0;
	txb[2] = (reg+1)<<4;
	txb[3] = adr;
	txb[4] = 0; // dummy byte
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1+4, txb, len, data, 0);

	return 0;
}

/**
 * Protect first 512KB
 */
alt_u32 sfProtect(void)
{
	alt_u8  txb[1+3+2];
	alt_u8  rxb[1];
	alt_u8  sts1;
	alt_u8  sts2;

	// Read Status Register 1
	txb[0] = 0x05;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);
	sts1 = rxb[0];

	// Read Status Register 2
	txb[0] = 0x35;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);
	sts2 = rxb[0];

	// set SEC(6)=0 TB(5)=1 BP(4-2)=100
	sts1 &= ~0x7C;
	sts1 |= 0x30;

	// write enable 0x06
	txb[0] = 0x06;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 0, rxb, 0);

	// Write Status Register 1 and 2 (0x01)
	txb[0] = 0x01;
	txb[1] = sts1;
	txb[2] = sts2;
	alt_avalon_spi_command(FLASH_SPI_BASE, 0, 3, txb, 0, rxb, 0);

	txb[0] = 0x05;
	do { alt_avalon_spi_command(FLASH_SPI_BASE, 0, 1, txb, 1, rxb, 0);} while (rxb[0]&1);
	return 0;
}

#endif /* SF_SECURITY */

/**
 *
 */
alt_u32 sfWrite(alt_u8* data, alt_u32 len, alt_u32 flg)
{
	if (flg & SF_WRFLG_ERASE) {
		sfErase(3, 0);
	}

	if (flg & SF_WRFLG_PROGRAM) {
		sfProgram(0x00000000, data, len);
	}

	if (flg & SF_WRFLG_VERIFY) {
		alt_u8 *buf = (alt_u8*)(0x00800000 + 0x00040000);
		int     ret;

		sfRead(0x00000000, buf, len);
#if 1
		ret = memcmp((char*)buf, (char*)data, len);
#else
		ret = memcmpr((char*)buf, (char*)data, len);
#endif
		if(ret){
			return -1;
		}
	}
	return 0;
}
