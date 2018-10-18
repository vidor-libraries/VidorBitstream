/*
 * sf.c
 *
 *  Created on: Apr 11, 2018
 *      Author: max
 */

#include <string.h>
#include <system.h>

#define SF_RPC_CMD        1
#define SF_SPI_IDX        0
#define SF_USE_QSPI       1
#define SF_SECURITY_CMDS  1

#if defined(SF_RPC_CMD) && (SF_RPC_CMD == 1)
  #include "mb.h"
#endif /* defined(SF_RPC_CMD) && (SF_RPC_CMD == 1) */

#include "tspi.h"
#include "sf.h"


#if defined(SF_USE_QSPI) && (SF_USE_QSPI == 1)

  #include <arduino_generic_quad_spi_controller2.h>
  #include <arduino_generic_quad_spi_controller2_regs.h>
  #include <io.h>
  #include <priv/alt_busy_sleep.h>

  #define QSPI_CSR_BASE  (0x80000000 | QSPI_AVL_CSR_BASE)
  #define QSPI_MEM_BASE  (0x80000000 | QSPI_AVL_MEM_BASE)


  /** When to time out a poll of the write in progress bit */
  /* 0.7 sec time out */
  #define QSPI_CONTROLLER_1US_TIMEOUT_VALUE        700000

#else  /*  defined(SF_USE_QSPI) && (SF_USE_QSPI == 1) */

  #define SF_FAST_RD

#endif  /*  defined(SF_USE_QSPI) && (SF_USE_QSPI == 1) */



alt_u32 sfSetup(alt_u32 cmd);
alt_u32 sfEnd(alt_u32 cmd);

/**
 *
 */
alt_u32 sfJedecId(void);
alt_u32 sfUniqueId(alt_u8* id);
alt_u32 sfErase(alt_u32 mode, alt_u32 adr);
alt_u32 sfProgram(alt_u32 adr, alt_u8* data, alt_u32 len);
alt_u32 sfRead(alt_u32 adr, alt_u8* data, alt_u32 len);

/**
 * Security Register
 */
#if defined(SF_SECURITY_CMDS) && (SF_SECURITY_CMDS == 1)
alt_u32 sfSRErase(alt_u8 reg);
alt_u32 sfSRProgram(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);
alt_u32 sfSRLock(alt_u8 reg);
alt_u32 sfSRRead(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len);
alt_u32 sfProtect(void);
#endif  /* defined(SF_SECURITY_CMDS) && (SF_SECURITY_CMDS == 1) */

alt_u32 sfEnableQuad(void);

#if defined(SF_USE_QSPI) && (SF_USE_QSPI == 1)
alt_32 static poll_for_wip(void);
#endif /* defined(SF_USE_QSPI) && (SF_USE_QSPI == 1) */

#if defined(SF_INVERT_BIT) && (SF_INVERT_BIT == 1)
void memcpyr(char *dst, char* src, int size);
int memcmpr(char *dst, char* src, int size);
#endif /* defined(SF_INVERT_BIT) && (SF_INVERT_BIT == 1) */

#if defined(SF_RPC_CMD) && (SF_RPC_CMD == 1)

/**
 */
void sfRpc(void)
{
  alt_u32 volatile *rpc = mbPtrGet();
  alt_u32 ret;

  ret = -1;
  if ((fpgaIp[RPC_GIID(rpc[0])].disc & 0xFFFFF) != SF_UID) {
    rpc[1] = ret;
    return ;
  }
  switch (RPC_PID(rpc[0])) {
  case 2:
    ret = sfSetup(rpc[0]);
    break;
  case 4:
    ret = sfEnd(rpc[0]);
    break;

  case 5:
    ret = sfJedecId();
    break;
  case 6:
    ret = sfUniqueId((alt_u8*)&rpc[2]);
    break;
  case 7:
    ret = sfErase(rpc[1], rpc[2]);
    break;
  case 8:
    ret = sfProgram(rpc[1], (alt_u8*)&rpc[3], rpc[2]);
    break;
  case 9:
    ret = sfRead(rpc[1], (alt_u8*)&rpc[3], rpc[2]);
    break;
#if defined(SF_SECURITY_CMDS) && (SF_SECURITY_CMDS == 1)
  case 10:
    ret = sfSRErase(rpc[1]);
    break;
  case 11:
    ret = sfSRProgram(rpc[1], rpc[2], (alt_u8*)&rpc[4], rpc[3]);
    break;
  case 12:
    ret = sfSRLock(rpc[1]);
    break;
  case 13:
    ret = sfSRRead(rpc[1], rpc[2], (alt_u8*)&rpc[4], rpc[3]);
    break;
  case 14:
    ret = sfProtect();
    break;
#endif  /* defined(SF_SECURITY_CMDS) && (SF_SECURITY_CMDS == 1) */
  case 15:
    ret = sfEnableQuad();
    break;
  }
  rpc[1] = ret;
}
#endif /* defined(SF_RPC_CMD) && (SF_RPC_CMD == 1) */

/**
 */
alt_u32 sfSetup(alt_u32 cmd)
{
  tspiSetup(SF_SPI_IDX);
  tspiModeSet(SF_SPI_IDX, 50000000, 0, 0, 1);

  return 0;
}

/**
 */
alt_u32 sfEnd(alt_u32 cmd)
{
  return 0;
}


/**
 * @return  MANUFACTURER ID DEVICE ID BYTE1 DEVICE ID BYTE2
 */
alt_u32 sfJedecId(void)
{
  alt_u8  txb[]={0x9f};
  alt_u8  rxb[4];

  tspiTrc(SF_SPI_IDX, 1, txb, 3, rxb);

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

  tspiTrc(SF_SPI_IDX, 1+4, txb, 8, id);
  return 0;
}

#if defined(SF_USE_QSPI) && (SF_USE_QSPI == 1)

/**
 */
alt_u32 sfErase(alt_u32 mode, alt_u32 adr)
{
  alt_u32 op_value = 0; /* value to write to EPCQ_MEM_OP register */

  switch(mode){
  case 2:
    /* 64KB Block Erase 64K Block address */

    /* write enable */
    IOWR_ARDUINO_QSPI_CONTROLLER2_MEM_OP(QSPI_CSR_BASE,
        ARDUINO_QSPI_CONTROLLER2_MEM_OP_WRITE_ENABLE_CMD);

    adr >>= 16;
    op_value = (adr << 8) & ARDUINO_QSPI_CONTROLLER2_MEM_OP_SECTOR_VALUE_MASK;
    op_value |= ARDUINO_QSPI_CONTROLLER2_MEM_OP_SECTOR_ERASE_CMD;

    /* write sector erase command to QSPI_MEM_OP register to erase sector "sector_number" */
    IOWR_ARDUINO_QSPI_CONTROLLER2_MEM_OP(QSPI_CSR_BASE, op_value);

    /* check whether erase triggered a illegal erase interrupt  */
    if((IORD_ARDUINO_QSPI_CONTROLLER2_ISR(QSPI_CSR_BASE) &
        ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_ERASE_MASK) ==
            ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_ERASE_ACTIVE) {
      /* clear register */
      /* QSPI_ISR access is write one to clear (W1C) */
      IOWR_ARDUINO_QSPI_CONTROLLER2_ISR(QSPI_CSR_BASE,
          ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_ERASE_MASK);
      return -1; /* erase failed, sector might be protected */
    }
    poll_for_wip();
    break;
  case 3:
    /* Chip Erase       Not used */
    /* write enable */
    IOWR_ARDUINO_QSPI_CONTROLLER2_MEM_OP(QSPI_CSR_BASE,
        ARDUINO_QSPI_CONTROLLER2_MEM_OP_WRITE_ENABLE_CMD);

    op_value = 0;
    op_value |= ARDUINO_QSPI_CONTROLLER2_MEM_OP_BULK_ERASE_CMD;

    /* write sector erase command to QSPI_MEM_OP register to erase sector "sector_number" */
    IOWR_ARDUINO_QSPI_CONTROLLER2_MEM_OP(QSPI_CSR_BASE, op_value);

    /* check whether erase triggered a illegal erase interrupt  */
    if((IORD_ARDUINO_QSPI_CONTROLLER2_ISR(QSPI_CSR_BASE) &
        ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_ERASE_MASK) ==
            ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_ERASE_ACTIVE) {
      /* clear register */
      /* QSPI_ISR access is write one to clear (W1C) */
      IOWR_ARDUINO_QSPI_CONTROLLER2_ISR(QSPI_CSR_BASE,
          ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_ERASE_MASK);
      return -1; /* erase failed, sector might be protected */
    }
    poll_for_wip();
    break;
  case 0: // Sector Erase      4K sector address
  case 1: // 32KB Block Erase 32K Block address
  default:
    return -1;
  }
  return 0;
}

/**
 */
alt_u32 sfProgram(alt_u32 adr, alt_u8* data, alt_u32 len)
{
  alt_u32 buffer_offset = 0; /** offset into data buffer to get write data */
  alt_u32 remaining_length = len; /** length left to write */
  alt_u32 write_offset = adr; /** offset into flash to write too */
  /*
   * Do writes one 32-bit word at a time.
   * We need to make sure that we pad the first few bytes so they're word aligned if they are
   * not already.
   */
  while (remaining_length > 0) {
    alt_u32 word_to_write = 0xFFFFFFFF; /** initialize word to write to blank word */
    alt_u32 padding = 0; /** bytes to pad the next word that is written */
    alt_u32 bytes_to_copy = sizeof(alt_u32); /** number of bytes from source to copy */

    /*
     * we need to make sure the write is word aligned
     * this should only be true at most 1 time
     */
    if (0 != (write_offset & (sizeof(alt_u32) - 1))) {
      /*
       * data is not word aligned
       * calculate padding bytes need to add before start of a data offset
       */
      padding = write_offset & (sizeof(alt_u32) - 1);

      /* update variables to account for padding being added */
      bytes_to_copy -= padding;

      if (bytes_to_copy > remaining_length) {
        bytes_to_copy = remaining_length;
      }

      write_offset = write_offset - padding;
      if (0 != (write_offset & (sizeof(alt_u32) - 1))) {
        return -1;
      }
    } else {
      if (bytes_to_copy > remaining_length) {
        bytes_to_copy = remaining_length;
      }
    }

    /* prepare the word to be written */
#if defined(SF_INVERT_BIT) && (SF_INVERT_BIT == 1)
    memcpyr((((void*)&word_to_write)) + padding, ((void*)data) + buffer_offset, bytes_to_copy);
#else
    memcpy((((void*)&word_to_write)) + padding, ((void*)data) + buffer_offset, bytes_to_copy);
#endif
    /* update offset and length variables */
    buffer_offset += bytes_to_copy;
    remaining_length -= bytes_to_copy;

    /* write to flash 32 bits at a time */
    IOWR_32DIRECT(QSPI_MEM_BASE, write_offset, word_to_write);

    /* check whether write triggered a illegal write interrupt */
    if ((IORD_ARDUINO_QSPI_CONTROLLER2_ISR(QSPI_CSR_BASE) &
         ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_WRITE_MASK) ==
                ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_WRITE_ACTIVE) {
      /* clear register */
      IOWR_ARDUINO_QSPI_CONTROLLER2_ISR(QSPI_CSR_BASE,
              ARDUINO_QSPI_CONTROLLER2_ISR_ILLEGAL_WRITE_MASK );
      return -1; /** write failed, sector might be protected */
    }
    poll_for_wip();

    /* update current offset */
    write_offset = write_offset + sizeof(alt_u32);
  }

  return 0;
}

/**
 */
alt_u32 sfRead(alt_u32 adr, alt_u8* data, alt_u32 len)
{
  memcpy(data, (void*)(QSPI_MEM_BASE+adr), len);
  return 0;
}

/**
 */
alt_32 static poll_for_wip(void)
{
  /* we'll want to implement timeout if a timeout value is specified */
#if QSPI_CONTROLLER_1US_TIMEOUT_VALUE > 0
  alt_u32 timeout = QSPI_CONTROLLER_1US_TIMEOUT_VALUE;
  alt_u16 counter = 0;
#endif

  /* while Write in Progress bit is set, we wait */
  while ((IORD_ARDUINO_QSPI_CONTROLLER2_STATUS(QSPI_CSR_BASE) &
      ARDUINO_QSPI_CONTROLLER2_STATUS_WIP_MASK) ==
      ARDUINO_QSPI_CONTROLLER2_STATUS_WIP_BUSY) {
    alt_busy_sleep(1); /* delay 1us */
#if QSPI_CONTROLLER_1US_TIMEOUT_VALUE > 0
    if (timeout <= counter) {
      return -1;
    }
    counter++;
#endif
  }
  return 0;
}

#else   /* defined(SF_USE_QSPI) && (SF_USE_QSPI == 1) */

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
  tspiTrc(SF_SPI_IDX, 1, txb, 0, rxb);

  switch(mode){
  case 0:
    // Sector Erase     4K sector address  0x020
    //adr >>= 12;
    txb[0] = 0x020;
    txb[1] = adr>>16;
    txb[2] = adr>>8;
    txb[3] = adr;
    txl = 4;
    break;
  case 1:
    // 32KB Block Erase 32K Block address   0x52
    //adr >>= 15;
    txb[0] = 0x52;
    txb[1] = adr>>16;
    txb[2] = adr>>8;
    txb[3] = adr;
    txl = 4;
    break;
  case 2:
    // 64KB Block Erase 64K Block address    0xD8
    //adr >>= 16;
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
  tspiTrc(SF_SPI_IDX, txl, txb, 0, rxb);

  // wait tCHPE 12/20/30 sec
  txb[0] = 0x05;
  do {tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);

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
    tspiTrc(SF_SPI_IDX, 1, txb, 0, rxb);

    //program      0x02 address (3 byte) data (1-256)
    //max data length is 256 byte
    cnt = (len+(adr & 0xFF))>256? (256-(adr & 0xFF)): len;

    txb[0] = 0x02;
    txb[1] = adr>>16;
    txb[2] = adr>>8;
    txb[3] = adr;
#if defined(SF_INVERT_BIT) && (SF_INVERT_BIT == 1)
    memcpyr((char*)txb+4, (char*)data+ptr, cnt);
#else
    memcpy((char*)txb+4, (char*)data+ptr, cnt);
#endif
    tspiTrc(SF_SPI_IDX, 1+3+cnt, txb, 0, rxb);
    len -= cnt;
    adr += cnt;
    ptr += cnt;

    txb[0] = 0x05;
    do { tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);
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
    tspiTrc(SF_SPI_IDX, 1+3+1, txb, cnt, data+ptr);
#else
    tspiTrc(SF_SPI_IDX, 1+3, txb, cnt, data+ptr);
#endif

    len -= cnt;
    adr += cnt;
    ptr += cnt;
  }while(len>0);

  return 0;
}

#endif /* defined(SF_USE_QSPI) && (SF_USE_QSPI == 1) */

#if defined(SF_SECURITY_CMDS) && (SF_SECURITY_CMDS == 1)

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

  txb[0] = 0x05;
  do { tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);

  //write enable 0x06
  txb[0] = 0x06;
  tspiTrc(SF_SPI_IDX, 1, txb, 0, rxb);

  //Erase Security Registers (44h)
  txb[0] = 0x44;
  txb[1] = 0;
  txb[2] = (reg+1)<<4;
  txb[3] = 0;

  tspiTrc(SF_SPI_IDX, 1+3, txb, 0, rxb);

  txb[0] = 0x05;
  do { tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);

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

  txb[0] = 0x05;
  do { tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);

  //write enable 0x06
  txb[0] = 0x06;
  tspiTrc(SF_SPI_IDX, 1, txb, 0, rxb);

  //Program Security Registers
  txb[0] = 0x42;
  txb[1] = 0;
  txb[2] = (reg+1)<<4;
  txb[3] = adr;
  memcpy((char*)txb+4, (char*)data, len);

  tspiTrc(SF_SPI_IDX, 1+3+len, txb, 0, rxb);

  txb[0] = 0x05;
  do { tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);

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
  tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);
  sts1 = rxb[0];

  // Read Status Register 2
  txb[0] = 0x35;
  tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);
  sts2 = rxb[0];

  // write enable 0x06
  txb[0] = 0x06;
  tspiTrc(SF_SPI_IDX, 1, txb, 0, rxb);

  // set LB bit
  sts2 |= 0x08 << reg;

  // Write Status Register 1 and 2 (0x01)
  txb[0] = 0x01;
  txb[1] = sts1;
  txb[2] = sts2;
  tspiTrc(SF_SPI_IDX, 3, txb, 0, rxb);

  txb[0] = 0x05;
  do { tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);

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
  tspiTrc(SF_SPI_IDX, 1+4, txb, len, data);

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
  tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);
  sts1 = rxb[0];

  // Read Status Register 2
  txb[0] = 0x35;
  tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);
  sts2 = rxb[0];

  // set SEC(6)=0 TB(5)=1 BP(4-2)=100
  sts1 &= ~0x7C;
  sts1 |= 0x30;

  // write enable 0x06
  txb[0] = 0x06;
  tspiTrc(SF_SPI_IDX, 1, txb, 0, rxb);

  // Write Status Register 1 and 2 (0x01)
  txb[0] = 0x01;
  txb[1] = sts1;
  txb[2] = sts2;
  tspiTrc(SF_SPI_IDX, 3, txb, 0, rxb);

  txb[0] = 0x05;
  do { tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);} while (rxb[0]&1);
  return 0;
}

#else /* defined(SF_SECURITY_CMDS) && (SF_SECURITY_CMDS == 1) */

alt_u32 sfSRErase(alt_u8 reg)
{
  return -1;
}
alt_u32 sfSRProgram(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len)
{
  return -1;
}
alt_u32 sfSRLock(alt_u8 reg)
{
  return -1;
}
alt_u32 sfSRRead(alt_u8 reg, alt_u8 adr, alt_u8* data, alt_u32 len)
{
  return -1;
}
alt_u32 sfProtect(void)
{
  return -1;
}

#endif /* defined(SF_SECURITY_CMDS) && (SF_SECURITY_CMDS == 1) */



/**
 */
alt_u32 sfEnableQuad(void)
{
  alt_u8  txb[3];
  alt_u8  rxb[2];

  //read status register 0
  txb[0] = 0x5;
  tspiTrc(SF_SPI_IDX, 1, txb, 1, rxb);

  //read status register 1
  txb[0] = 0x35;
  tspiTrc(SF_SPI_IDX, 1, txb, 1, &rxb[1]);

  // if QE bit is not set enable it using method for
  if ((rxb[1] & 2) == 0) {
   txb[0] = 0x06;
   tspiTrc(SF_SPI_IDX, 1, txb, 0, rxb);
   txb[0] = 0x1;
   txb[1] = rxb[0];
   txb[2] = rxb[1]|2;
   tspiTrc(SF_SPI_IDX, 3, txb, 0, rxb);
  }

  return 0;
}


#if defined(SF_INVERT_BIT) && (SF_INVERT_BIT == 1)
/**
 *
 */
void memcpyr(char *dst, char* src, int size)
{
  char v;
  while (size>0) {
    v = *src++;
    v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
    v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
    v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
    *dst++ = v;
    size--;
  }
}

/**
 *
 */
int memcmpr(char *dst, char* src, int size)
{
  char v;
  int  p;
  p = 0;
  while (size>0) {
    v = *src++;
    v = ((v >> 1) & 0x55) | ((v & 0x55) << 1);
    v = ((v >> 2) & 0x33) | ((v & 0x33) << 2);
    v = ((v >> 4) & 0x0F) | ((v & 0x0F) << 4);
    p++;
    if (*dst++ != v) {
      return p;
    }
    size--;
  }
  return 0;
}
#endif /* defined(SF_INVERT_BIT) && (SF_INVERT_BIT == 1) */
