module MKRVIDOR4000_graphics_top
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
  .iDVI_HPD(iDVI_HPD)
);


wire fb_st_start,fb_st_dv,fb_st_ready;
wire [30:0] fb_st_data;

wire [23:0] mipi_st_data;
wire mipi_st_start,mipi_st_dv;

wire [23:0] mipi_ste_data;
wire mipi_ste_start,mipi_ste_dv;

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

wire wMIPI_SCL_O, wMIPI_SCL_EN;
wire wMIPI_SDA_O, wMIPI_SDA_EN;
wire wHDMI_SCL_O, wHDMI_SCL_EN;
wire wHDMI_SDA_O, wHDMI_SDA_EN;

wire [1:0] wIRQ_OUT;

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

assign bHDMI_SDA = wHDMI_SDA_EN ? wHDMI_SDA_O : 1'bz;
assign bHDMI_SCL = wHDMI_SCL_EN ? wHDMI_SCL_O : 1'bz;

assign bMIPI_SDA = wMIPI_SDA_EN ? wMIPI_SDA_O : 1'bz;
assign bMIPI_SCL = wMIPI_SCL_EN ? wMIPI_SCL_O : 1'bz;

MKRVIDOR4000_graphics_sys u0(
		.clk_clk                (wMEM_CLK),               //      clk.clk
		.reset_reset_n          (rRESETCNT[5]), // reset.reset_n
		.vid_clk                (wVID_CLK),        //   vid.clk
		.clk_0_clk              (wFLASH_CLK),
//		.reset_0_reset_n			(rRESETCNT[5]), // reset.reset_n

		.sdram_addr             (oSDRAM_ADDR), //    sdram.addr
		.sdram_ba               (oSDRAM_BA),   //         .ba
		.sdram_cas_n            (oSDRAM_CASn), //         .cas_n
		.sdram_cke              (oSDRAM_CKE),  //         .cke
		.sdram_cs_n             (oSDRAM_CSn),  //         .cs_n
		.sdram_dq               (bSDRAM_DQ),   //         .dq
		.sdram_dqm              (oSDRAM_DQM),  //         .dqm
		.sdram_ras_n            (oSDRAM_RASn), //         .ras_n
		.sdram_we_n             (oSDRAM_WEn),   //         .we_n

		.fb_vport_red           (wDVI_RED),     //      .red
		.fb_vport_grn           (wDVI_GRN),     //      .grn
 		.fb_vport_blu           (wDVI_BLU),     // vport.blu
		.fb_vport_de            (wDVI_DE),      //      .de
		.fb_vport_hs            (wDVI_HS),      //      .hs
		.fb_vport_vs            (wDVI_VS),      //      .vs
		.hdmi_i2c_scl_i         (bHDMI_SCL),    //   hdmi_i2c.scl_i
		.hdmi_i2c_scl_o         (wHDMI_SCL_O),    //           .scl_o
		.hdmi_i2c_scl_en        (wHDMI_SCL_EN),   //           .scl_en
		.hdmi_i2c_sda_i         (bHDMI_SDA),    //           .sda_i
		.hdmi_i2c_sda_o         (wHDMI_SDA_O),    //           .sda_o
		.hdmi_i2c_sda_en        (wHDMI_SDA_EN),   //           .sda_en

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

		.csi_i2c_scl_i          (bMIPI_SCL),    //   hdmi_i2c.scl_i
		.csi_i2c_scl_o          (wMIPI_SCL_O),    //           .scl_o
		.csi_i2c_scl_en         (wMIPI_SCL_EN),   //           .scl_en
		.csi_i2c_sda_i          (bMIPI_SDA),    //           .sda_i
		.csi_i2c_sda_o          (wMIPI_SDA_O),    //           .sda_o
		.csi_i2c_sda_en         (wMIPI_SDA_EN),   //           .sda_en

		.mipi_rx_clk            (iMIPI_CLK),
		.mipi_rx_data           (iMIPI_D),

		.arb_fb_clk             (wVID_CLK),       //    arb_fb.clk
		.arb_fb_rdy             (fb_st_ready),       //          .rdy
		.arb_fb_data            (fb_st_data),      //          .data
		.arb_fb_dv              (fb_st_dv),        //          .dv
		.arb_fb_start           (fb_st_start),     //          .start

		.fb_st_start            (fb_st_start),      //     fb_st.start
		.fb_st_data             (fb_st_data),       //          .data
		.fb_st_dv               (fb_st_dv),         //          .dv
		.fb_st_ready            (fb_st_ready),       //          .ready

		.mipi_st_data           (mipi_st_data),     //   mipi_st.data
		.mipi_st_start          (mipi_st_start),    //          .start
		.mipi_st_dv             (mipi_st_dv),       //          .dv
		.qr_vid_in_reset        (0),  //           .reset
		.qr_vid_in_clk          (iMIPI_CLK),    //           .clk
		.qr_vid_in_data         (mipi_st_data),   //  qr_vid_in.data
		.qr_vid_in_dv           (mipi_st_dv),     //           .dv
		.qr_vid_in_start        (mipi_st_start),  //           .start
		.qr_vid_out_data        (mipi_ste_data),  // qr_vid_out.data
		.qr_vid_out_dv          (mipi_ste_dv),    //           .dv
		.qr_vid_out_start       (mipi_ste_start),   //           .start
		.arb_mipi_clk           (iMIPI_CLK),     //  arb_mipi.clk
		.arb_mipi_data          ({mipi_ste_data[23-:5],mipi_ste_data[15-:5],mipi_ste_data[7-:5]}),    //          .data
		.arb_mipi_start         (mipi_ste_start),    //          .data
		.arb_mipi_dv            (mipi_ste_dv),      //          .dv

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
		.irq_in_port            (iSAM_INT),      //        irq.in_port
		.irq_out_port           (wIRQ_OUT),     //           .out_port

		.sam_pwm_pwm            (wSAM_OUT1),
		.neopixel_data				  (wSAM_OUT2[11:1]),
		.neopixel_clock				  (wSAM_OUT2[0]),
		.encoder_encoder_a      ({bMKR_D[13],bMKR_D[11],bMKR_D[9],bMKR_D[7],bMKR_D[5],bMKR_D[3],bMKR_D[1],bMKR_A[6],bMKR_A[4],bMKR_A[2],bMKR_A[0]}), //    encoder.encoder_a
		.encoder_encoder_b      ({bMKR_D[14],bMKR_D[12],bMKR_D[10],bMKR_D[8],bMKR_D[6],bMKR_D[4],bMKR_D[2],bMKR_D[0],bMKR_A[5],bMKR_A[3],bMKR_A[1]})  //           .encoder_b

	);
assign wSAM_OUT2[22:12] = wSAM_OUT2[11:1];
assign oSAM_INT         = wIRQ_OUT[1];
assign wWM_OUT2[11]     = wNINA_SS;
assign wWM_OUT2[18]     = wNINA_SCLK;
assign wWM_OUT2[19]     = wNINA_MOSI;
`ifndef FREE_VERSION
assign wWM_OUT2[16]     = wNINA_RX;
`endif

assign wWM_OUT1[15]     = wSAM_PIO_IN[22]; // D14 for NINA_RX in bypass
assign wWM_OUT1[0]      = wSAM_PIO_IN[15]; // D7 for NINA_RESET in bypass
assign wWM_OUT1[10]     = wSAM_PIO_IN[14]; // D6 for NINA_GPIO0 in bypass

assign wNINA_MISO       = wWM_PIO_IN[20]; // route NINA_MISO to internal SPI

// MIPI input
assign bMIPI_GP[0]=1'b1;
assign bMIPI_GP[1]=1'bz;

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
