module MKRVIDOR4000_template_mbox_top
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

assign wVID_CLK   = wCLK24;
assign wVID_CLKx5 = wCLK120;
assign wCLK8      = iCLK;

// internal oscillator
cyclone10lp_oscillator   osc
	(
	.clkout(wOSC_CLK),
	.oscena(1'b1));

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

wire [31:0] wSAM_PIO_IN;
wire [31:0] wSAM_PIO_OUT;
wire [31:0] wSAM_PIO_DIR;
wire [63:0] wSAM_PIO_MSEL;
wire [31:0] wWM_PIO_IN;
wire [31:0] wWM_PIO_OUT;
wire [31:0] wWM_PIO_DIR;
wire [63:0] wWM_PIO_MSEL;
wire [31:0] wPEX_PIO_IN;
wire [31:0] wPEX_PIO_OUT;
wire [31:0] wPEX_PIO_DIR;
wire [63:0] wPEX_PIO_MSEL;

reg [5:0] rRESETCNT;

always @(posedge wMEM_CLK)
begin
  if (!rRESETCNT[5])
  begin
  rRESETCNT<=rRESETCNT+1;
  end
end

wire [3:0] wQSPI_DATAOUT;
wire [3:0] wQSPI_DATAOE;
assign oFLASH_SCK  = wFLASH_SCK&!wFLASH_CS|wQSPI_CLK&!wQSPI_OE&!wQSPI_NCS;
assign oFLASH_HOLD = !wQSPI_OE&wQSPI_DATAOE[3]&!wQSPI_NCS ? wQSPI_DATAOUT[3] : wFLASH_CS ? 1'bz : 1'b1;
assign oFLASH_WP   = !wQSPI_OE&wQSPI_DATAOE[2]&!wQSPI_NCS ? wQSPI_DATAOUT[2] : wFLASH_CS ? 1'bz : 1'b1;
assign iFLASH_MISO = !wQSPI_OE&wQSPI_DATAOE[1]&!wQSPI_NCS ? wQSPI_DATAOUT[1] : 1'bz;
assign oFLASH_MOSI = !wQSPI_OE&wQSPI_DATAOE[0]&!wQSPI_NCS ? wQSPI_DATAOUT[0] : wFLASH_CS ? 1'bz : wFLASH_MOSI;
assign oFLASH_CS   = wQSPI_NCS & wFLASH_CS;

`ifndef FREE_VERSION
MKRVIDOR4000_template_mbox_sys u0(
`else
MKRVIDOR4000_template_mbox_lite_sys u0(
`endif
		.clk_clk                (wMEM_CLK),               //      clk.clk
		.reset_reset_n          (rRESETCNT[5]), // reset.reset_n
		.clk_0_clk              (wFLASH_CLK),

		.sdram_addr             (oSDRAM_ADDR), //    sdram.addr
		.sdram_ba               (oSDRAM_BA),   //         .ba
		.sdram_cas_n            (oSDRAM_CASn), //         .cas_n
		.sdram_cke              (oSDRAM_CKE),  //         .cke
		.sdram_cs_n             (oSDRAM_CSn),  //         .cs_n
		.sdram_dq               (bSDRAM_DQ),   //         .dq
		.sdram_dqm              (oSDRAM_DQM),  //         .dqm
		.sdram_ras_n            (oSDRAM_RASn), //         .ras_n
		.sdram_we_n             (oSDRAM_WEn),   //         .we_n

		.flash_spi_MISO         (iFLASH_MISO ),   // flash_spi.MISO
		.flash_spi_MOSI         (wFLASH_MOSI),   //          .MOSI
		.flash_spi_SCLK         (wFLASH_SCK),   //          .SCLK
		.flash_spi_CS           (wFLASH_CS),   //          .SS_n

		.qspi_dclk              (wQSPI_CLK),       //      qspi.dclk
		.qspi_ncs               (wQSPI_NCS),        //          .ncs
		.qspi_oe                (wQSPI_OE),         //          .oe
		.qspi_dataout           (wQSPI_DATAOUT),    //          .dataout
		.qspi_dataoe            (wQSPI_DATAOE),     //          .dataoe
		.qspi_datain            ({oFLASH_HOLD, oFLASH_WP, iFLASH_MISO, oFLASH_MOSI}),      //          .datain

		.nina_spi_MISO          (wNINA_MISO),    //  nina_spi.MISO
		.nina_spi_MOSI          (wNINA_MOSI),    //          .MOSI
		.nina_spi_SCLK          (wNINA_SCLK),    //          .SCLK
		.nina_spi_CS            (wNINA_SS),     //          .SS_n

`ifndef FREE_VERSION
		.nina_uart_sin          (wWM_PIO_IN[16]),     //  nina_uart.sin
		.nina_uart_sout         (wNINA_RX),    //           .sout
`endif

		.sam_pio_in             (wSAM_PIO_IN),       //   sam_pio.in
		.sam_pio_out            (wSAM_PIO_OUT),      //          .out
		.sam_pio_dir            (wSAM_PIO_DIR),      //          .dir
		.sam_pio_msel           (wSAM_PIO_MSEL),     //          .msel

		.wm_pio_in              (wWM_PIO_IN),        //    wm_pio.in
		.wm_pio_out             (wWM_PIO_OUT),       //          .out
		.wm_pio_dir             (wWM_PIO_DIR),       //          .dir
		.wm_pio_msel            (wWM_PIO_MSEL),       //          .msel

		.pex_pio_in             (wPEX_PIO_IN),       //   pex_pio.in
		.pex_pio_out            (wPEX_PIO_OUT),      //          .out
		.pex_pio_dir            (wPEX_PIO_DIR),      //          .dir
		.pex_pio_msel           (wPEX_PIO_MSEL),     //          .msel

    .mb_rq                  (iSAM_INT),      //        iMST_RQ
		.mb_ak                  (oSAM_INT),      //        oMST_AK

		.sam_pwm_pwm            (wSAM_OUT1)

	);

assign wWM_OUT2[11]     = wNINA_SS;
assign wWM_OUT2[18]     = wNINA_SCLK;
assign wWM_OUT2[19]     = wNINA_MOSI;
`ifndef FREE_VERSION
assign wWM_OUT2[15]     = wNINA_RX;
`endif

assign wWM_OUT1[15]     = wSAM_PIO_IN[22]; // D14 for NINA_RX in bypass
assign wWM_OUT1[0]      = wSAM_PIO_IN[15]; // D7 for NINA_RESET in bypass
assign wWM_OUT1[10]     = wSAM_PIO_IN[14]; // D6 for NINA_GPIO0 in bypass

assign wNINA_MISO       = wWM_PIO_IN[20]; // route NINA_MISO to internal SPI


wire        wDUMMY1,wDUMMY0;
wire [31:0] wSAM_PIN_OUT,wSAM_OUT1,wSAM_OUT2,wSAM_OUT3;
wire [31:0] wWM_PIN_OUT,wWM_OUT1,wWM_OUT2,wWM_OUT3;
wire [31:0] wPEX_PIN_OUT,wPEX_OUT1,wPEX_OUT2,wPEX_OUT3;

assign wSAM_PIO_IN = {bMKR_D,bMKR_A,bMKR_AREF};
assign wWM_PIO_IN = {bWM_PIO1,bWM_PIO36,bWM_PIO29,iWM_PIO32,iWM_TX,oWM_RX,bWM_PIO35,bWM_PIO34,bWM_PIO31,bWM_PIO28,bWM_PIO27,bWM_PIO21,bWM_PIO20,bWM_PIO18,bWM_PIO8,bWM_PIO7,bWM_PIO5,bWM_PIO4,bWM_PIO3,bWM_PIO2,oWM_RESET};
assign wPEX_PIO_IN = {iPEX_PIN33,iPEX_PIN31,iPEX_PIN25,iPEX_PIN23,iPEX_PIN13,iPEX_PIN11,bPEX_PIN51,bPEX_PIN49,bPEX_PIN48,bPEX_PIN47,bPEX_PIN46,bPEX_PIN45,bPEX_PIN44,bPEX_PIN42,bPEX_PIN32,bPEX_PIN30,bPEX_PIN28,bPEX_PIN20,bPEX_PIN16,bPEX_PIN14,bPEX_PIN12,bPEX_PIN10,bPEX_PIN8,bPEX_PIN6,bPEX_RST};

assign {bMKR_D,bMKR_A,bMKR_AREF}= wSAM_PIN_OUT;
assign {bWM_PIO1,bWM_PIO36,bWM_PIO29,wDUMMY1,wDUMMY0,oWM_RX,bWM_PIO35,bWM_PIO34,bWM_PIO31,bWM_PIO28,bWM_PIO27,bWM_PIO21,bWM_PIO20,bWM_PIO18,bWM_PIO8,bWM_PIO7,bWM_PIO5,bWM_PIO4,bWM_PIO3,bWM_PIO2,oWM_RESET} = wWM_PIN_OUT;
assign {bPEX_PIN51,bPEX_PIN49,bPEX_PIN48,bPEX_PIN47,bPEX_PIN46,bPEX_PIN45,bPEX_PIN44,bPEX_PIN42,bPEX_PIN32,bPEX_PIN30,bPEX_PIN28,bPEX_PIN20,bPEX_PIN16,bPEX_PIN14,bPEX_PIN12,bPEX_PIN10,bPEX_PIN8,bPEX_PIN6,bPEX_RST} = wPEX_PIN_OUT;
genvar i;
generate

for (i=0;i<31;i++) begin : genloop
assign wSAM_PIN_OUT[i] =  (wSAM_PIO_MSEL[i*2+1-:2] ==0) ?
                            (i==18 && wWM_PIO_MSEL[20*2+1-:2]==1) ? wWM_PIO_IN[20] : // NINA_MISO to SAM (D10) in bypass
                            (i==21 && wWM_PIO_MSEL[15*2+1-:2]==1) ? wWM_PIO_IN[15] : // NINA_TX to SAM (D13) in bypass
                            (i==12 && wWM_PIO_MSEL[5*2+1-:2]==1) ? wWM_PIO_IN[5] : // ACK from NINA to SAM (D4) in bypass
                            !wSAM_PIO_DIR[i] ? 1'bZ :wSAM_PIO_OUT[i] :
                          (wSAM_PIO_MSEL[i*2+1-:2] ==1) ? wSAM_OUT1[i] :
                          (wSAM_PIO_MSEL[i*2+1-:2] ==2) ? wSAM_OUT2[i] :
                          (wSAM_PIO_MSEL[i*2+1-:2] ==3) ? wSAM_OUT3[i] : 0;

assign wWM_PIN_OUT[i] =   (wWM_PIO_MSEL[i*2+1-:2] ==0) ? !wWM_PIO_DIR[i] ? 1'bZ : wWM_PIO_OUT[i] :
                          (wWM_PIO_MSEL[i*2+1-:2] ==1) ? wWM_OUT1[i] :
                          (wWM_PIO_MSEL[i*2+1-:2] ==2) ? wWM_OUT2[i] :
                          (wWM_PIO_MSEL[i*2+1-:2] ==3) ? wWM_OUT3[i] : 0;

assign wPEX_PIN_OUT[i] =  (wPEX_PIO_MSEL[i*2+1-:2] ==0) ? !wPEX_PIO_DIR[i] ? 1'bZ : wPEX_PIO_OUT[i] :
                          (wPEX_PIO_MSEL[i*2+1-:2] ==1) ? wPEX_OUT1[i] :
                          (wPEX_PIO_MSEL[i*2+1-:2] ==2) ? wPEX_OUT2[i] :
                          (wPEX_PIO_MSEL[i*2+1-:2] ==3) ? wPEX_OUT3[i] : 0;

end
endgenerate

endmodule
