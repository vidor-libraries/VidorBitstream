/*
* Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
* This file is part of Vidor IP.
* Copyright (c) 2018
* Authors: Dario Pennisi
*
* This software is released under:
* The GNU General Public License, which covers the main part of 
* Vidor IP
* The terms of this license can be found at:
* https://www.gnu.org/licenses/gpl-3.0.en.html
*
* You can be released from the requirements of the above licenses by purchasing
* a commercial license. Buying such a license is mandatory if you want to modify or
* otherwise use the software for commercial activities involving the Arduino
* software without disclosing the source code of your own applications. To purchase
* a commercial license, send an email to license@arduino.cc.
*
*/

module MKRVIDOR4000_top
(
  // system signals
  input         iCLK,
  input         iRESETn,
  output        iSAM_INT,
  input         oSAM_INT,
  
  // SDRAM
  output        oSDRAM_CLK,
  output [11:0] oSDRAM_ADDR,
  output [1:0]  oSDRAM_BA,
  output        oSDRAM_CASn,
  output        oSDRAM_CKE,
  output        oSDRAM_CSn,
  inout  [15:0] bSDRAM_DQ,
  output [1:0]  oSDRAM_DQM,
  output        oSDRAM_RASn,
  output        oSDRAM_WEn,

  // SAM D21 PINS
  inout         bMKR_AREF,
  inout  [6:0]  bMKR_A,
  inout  [14:0] bMKR_D,
  
  // Mini PCIe
  inout         bPEX_RST,
  inout         bPEX_PIN6,
  inout         bPEX_PIN8,
  inout         bPEX_PIN10,
  input         iPEX_PIN11,
  inout         bPEX_PIN12,
  input         iPEX_PIN13,
  inout         bPEX_PIN14,
  inout         bPEX_PIN16,
  inout         bPEX_PIN20,
  input         iPEX_PIN23,
  input         iPEX_PIN25,
  inout         bPEX_PIN28,
  inout         bPEX_PIN30,
  input         iPEX_PIN31,
  inout         bPEX_PIN32,
  input         iPEX_PIN33,
  inout         bPEX_PIN42,
  inout         bPEX_PIN44,
  inout         bPEX_PIN45,
  inout         bPEX_PIN46,
  inout         bPEX_PIN47,
  inout         bPEX_PIN48,
  inout         bPEX_PIN49,
  inout         bPEX_PIN51,

  // NINA interface
  inout         bWM_PIO1,
  inout         bWM_PIO2,
  inout         bWM_PIO3,
  inout         bWM_PIO4,
  inout         bWM_PIO5,
  inout         bWM_PIO7,
  inout         bWM_PIO8,
  inout         bWM_PIO18,
  inout         bWM_PIO20,
  inout         bWM_PIO21,
  inout         bWM_PIO27,
  inout         bWM_PIO28,
  inout         bWM_PIO29,
  inout         bWM_PIO31,
  input         iWM_PIO32,
  inout         bWM_PIO34,
  inout         bWM_PIO35,
  inout         bWM_PIO36,
  input         iWM_TX,
  inout         oWM_RX,
  inout         oWM_RESET,

  // HDMI output
  output [2:0]  oHDMI_TX,
  output        oHDMI_CLK,

  inout         bHDMI_SDA,
  inout         bHDMI_SCL,
  
  input         iHDMI_HPD,
  
  // MIPI input
  input  [1:0]  iMIPI_D,
  input         iMIPI_CLK,
  inout         bMIPI_SDA,
  inout         bMIPI_SCL,
  inout  [1:0]  bMIPI_GP,

  // Q-SPI Flash interface
  output        oFLASH_SCK,
  output        oFLASH_CS,
  inout         oFLASH_MOSI,
  inout         iFLASH_MISO,
  inout         oFLASH_HOLD,
  inout         oFLASH_WP

);

// signal declaration

wire        wCLK8,wCLK24, wCLK64, wCLK120;

wire [31:0] wJTAG_ADDRESS, wJTAG_READ_DATA, wJTAG_WRITE_DATA, wDPRAM_READ_DATA;
wire        wJTAG_READ, wJTAG_WRITE, wJTAG_WAIT_REQUEST, wJTAG_READ_DATAVALID;
wire [4:0]  wJTAG_BURST_COUNT;
wire        wDPRAM_CS;

wire [7:0]  wDVI_RED,wDVI_GRN,wDVI_BLU;
wire        wDVI_HS, wDVI_VS, wDVI_DE;

wire        wVID_CLK, wVID_CLKx5;
wire        wMEM_CLK;

assign wVID_CLK   = wCLK24;
assign wVID_CLKx5 = wCLK120;
assign wCLK8      = iCLK;

// system PLL
SYSTEM_PLL PLL_inst(
  .areset(1'b0),
  .inclk0(wCLK8),
  .c0(wCLK24),
  .c1(wCLK120),
  .c2(wMEM_CLK),
  .c3(oSDRAM_CLK),
  .c4(wFLASH_CLK),
   
  .locked());


reg [5:0] rRESETCNT;

always @(posedge wMEM_CLK)
begin
  if (!rRESETCNT[5])
  begin
  rRESETCNT<=rRESETCNT+1;
  end
end

//ACK:        PA28           <-  nina pin 7   <= esp gpio 33
//GPIO:       PA07 (MKR_A6)   -> nina pin 27  <= esp gpio 0
//NINA_RESET: PA06 (MKR_A5)   -> nina reset   <= esp reset
//CIPO:       PA19 (MKR_D10) <-  nina pin 1   <= esp gpio 23
//COPI:       PA16 (MKR_D8)   -> nina pin 36  <= esp gpio 12
//SCK:        PA17 (MKR_D9)   -> nina pin 29  <= esp gpio 18
//CS:         PB9             -> nina pin 28  <= esp gpio 5
//UART_TX:    PB22 (MKR_D14)  -> nina pin 23  <= esp gpio 3
//UART_RX:    PB23 (MKR_D13) <-  nina pin 22  <= esp gpio 1
//SDA:        PA08 (MKR_D11) <-> nina pin 35  <= esp gpio 13
//SCL:        PA09 (MKR_D12) <-  nina pin 31  <= esp gpio 14

assign bMKR_D[11] = 1'bZ;
assign bMKR_D[12] = 1'bz;
assign bWMPIO35 = 1'bZ;
assign bWMPIO31 = 1'bZ;

assign bMKR_D[8] = 1'bz;
assign bMKR_D[9] = 1'bz;
assign bWM_PIO1  = 1'bZ;
assign bMKR_A[5] = 1'bZ;
assign bWM_PIO7  = 1'bZ;
assign bMKR_A[6] = 1'bZ;


// UART Bridge
assign bMKR_D[13] = iWM_TX;
assign oWM_RX = bMKR_D[14];

// SPI bridge

assign bMKR_D[10] = bWM_PIO1; //CIPO
assign bWM_PIO36 =  bMKR_D[8]; //COPI
assign bWM_PIO29 = bMKR_D[9]; //SCK
assign bWM_PIO28 = oSAM_INT; // CS (PB9)

// reset
assign oWM_RESET = bMKR_A[5];

// ack
assign iSAM_INT =   bWM_PIO7; //PA28
//GPIO
assign bWM_PIO27 = bMKR_A[6]; //PA07

endmodule
