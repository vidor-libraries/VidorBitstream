module MKRVIDOR4000_pacman_top
(
  // system signals
  input         iCLK,
  input         iRESETn,

  input         iSAM_INT,
  output        oSAM_INT,

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

  // S-SPI Flash interface
//  output        oFLASH_MOSI,
//  input         iFLASH_MISO,
//  output        oFLASH_SCK,
//  output        oFLASH_CS,
//  output        oFLASH_WP,
//  output        oFLASH_HOLD

  // Q-SPI Flash interface
  output        oFLASH_SCK,
  output        oFLASH_CS,
  inout         oFLASH_MOSI,
  inout         iFLASH_MISO,
  inout         oFLASH_HOLD,
  inout         oFLASH_WP

);

// signal declaration

wire        wOSC_CLK;

wire        wCLK8,wCLK24, wCLK64, wCLK120;

wire [31:0] wJTAG_ADDRESS, wJTAG_READ_DATA, wJTAG_WRITE_DATA, wDPRAM_READ_DATA;
wire        wJTAG_READ, wJTAG_WRITE, wJTAG_WAIT_REQUEST, wJTAG_READ_DATAVALID;
wire [4:0]  wJTAG_BURST_COUNT;
wire        wDPRAM_CS;

wire [7:0]  wDVI_RED,wDVI_GRN,wDVI_BLU;
wire        wDVI_HS, wDVI_VS, wDVI_DE;

wire        wVID_CLK, wVID_CLKx5;
wire        wMEM_CLK;


// DVI output
DVI_OUT
(
  .iPCLK(wVID_CLK),
  .iSCLK(wVID_CLKx5),

  .iRED(wDVI_RED),
  .iGRN(wDVI_GRN),
  .iBLU(wDVI_BLU),
  .iHS (wDVI_HS),
  .iVS (wDVI_VS),
  .iDE (wDVI_DE),

  .oDVI_DATA(oHDMI_TX),
  .oDVI_CLK(oHDMI_CLK),
  .iDVI_HPD(iHDMI_HPD)
);

reg [5:0] rRESETCNT;

always @(posedge iCLK)
begin
  if (!rRESETCNT[5])
  begin
  rRESETCNT<=rRESETCNT+1;
  end
end
wire [7:0] wAUDIO;

PACMAN pm_inst (
    .O_VIDEO_R             (wDVI_RED[7:5]),
    .O_VIDEO_G             (wDVI_GRN[7:5]),
    .O_VIDEO_B             (wDVI_BLU[7:6]),
    .O_HSYNC               (wDVI_HS),
    .O_VSYNC               (wDVI_VS),
	 .O_DE                  (wDVI_DE),
	 .O_DVI_CLK             (wVID_CLKx5),
	 .O_VID_CLK             (wVID_CLK),

    .O_AUDIO             (wAUDIO),

    .I_SW                  ({bMKR_D[6:4],1'b1}),
    .I_BUTTON              (bMKR_D[3:0]),
    .O_LED                 (),

    .I_RESET               (!rRESETCNT[5]),
    .I_CLK_REF             (iCLK)
    );

SD_MODULATOR #(
  .pBITS(8),
  .pINTERP_BITS(5)
) audio_l
(
  .iCLK(iCLK),
  .iDATA(wAUDIO),
  .oSTROBE(),
  .oDAC(bMKR_A[0])
);

endmodule
