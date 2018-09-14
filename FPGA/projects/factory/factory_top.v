module factory_top
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

wire fb_st_start,fb_st_dv,fb_st_ready;
wire [30:0] fb_st_data;

assign wVID_CLK   = wCLK24;
assign wVID_CLKx5 = wCLK120;
assign wCLK8      = iCLK;

wire [1:0] wIRQ_OUT;

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
	.c4(wQSPI_CLK),
   
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
  .oDVI_CLK(oHDMI_CLK)
);


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

assign oFLASH_SCK=wFLASH_SCK&!wFLASH_CS|wQSPI_CLK&!wQSPI_OE&!wQSPI_NCS;
assign oFLASH_HOLD = !wQSPI_OE&wQSPI_DATAOE[3]&!wQSPI_NCS ? wQSPI_DATAOUT[3] : 1'b1;
assign oFLASH_WP = !wQSPI_OE&wQSPI_DATAOE[2]&!wQSPI_NCS ? wQSPI_DATAOUT[2] : 1'b1;
assign iFLASH_MISO = !wQSPI_OE&wQSPI_DATAOE[1]&!wQSPI_NCS ? wQSPI_DATAOUT[1] : 1'bz;
assign oFLASH_MOSI = !wQSPI_OE&wQSPI_DATAOE[0]&!wQSPI_NCS ? wQSPI_DATAOUT[0] : wFLASH_MOSI;
assign oFLASH_CS= wQSPI_NCS & wFLASH_CS;

factory u0(
		.clk_clk                (wMEM_CLK),               //      clk.clk
		.reset_reset_n          (rRESETCNT[5]), // reset.reset_n
		.vid_clk                (wVID_CLK),        //   vid.clk
		.clk_0_clk              (wQSPI_CLK),
		.reset_0_reset_n	  		(rRESETCNT[5]), // reset.reset_n

		.sdram_addr             (oSDRAM_ADDR), //    sdram.addr
		.sdram_ba               (oSDRAM_BA),   //         .ba
		.sdram_cas_n            (oSDRAM_CASn), //         .cas_n
		.sdram_cke              (oSDRAM_CKE),  //         .cke
		.sdram_cs_n             (oSDRAM_CSn),  //         .cs_n
		.sdram_dq               (bSDRAM_DQ),   //         .dq
		.sdram_dqm              (oSDRAM_DQM),  //         .dqm
		.sdram_ras_n            (oSDRAM_RASn), //         .ras_n
		.sdram_we_n             (oSDRAM_WEn),   //         .we_n

 		.fb_vport_blu           (wDVI_BLU),     // vport.blu
		.fb_vport_de            (wDVI_DE),      //      .de
		.fb_vport_grn           (wDVI_GRN),     //      .grn
		.fb_vport_hs            (wDVI_HS),      //      .hs
		.fb_vport_vs            (wDVI_VS),      //      .vs
		.fb_vport_red           (wDVI_RED),     //      .red

		.flash_spi_MISO         (iFLASH_MISO ),   // flash_spi.MISO
		.flash_spi_MOSI         (wFLASH_MOSI),   //          .MOSI
		.flash_spi_SCLK         (wFLASH_SCK),   //          .SCLK
		.flash_spi_SS_n         (wFLASH_CS),   //          .SS_n

		.qspi_dclk              (wQSPI_CLK),       //      qspi.dclk
		.qspi_ncs               (wQSPI_NCS),        //          .ncs
		.qspi_oe                (wQSPI_OE),         //          .oe
		.qspi_dataout           (wQSPI_DATAOUT),    //          .dataout
		.qspi_dataoe            (wQSPI_DATAOE),     //          .dataoe
		.qspi_datain            ({oFLASH_HOLD, oFLASH_WP, iFLASH_MISO, oFLASH_MOSI}),      //          .datain

		.arb_fb_clk             (wVID_CLK),       //    arb_fb.clk
		.arb_fb_rdy             (fb_st_ready),       //          .rdy
		.arb_fb_data            (fb_st_data),      //          .data
		.arb_fb_dv              (fb_st_dv),        //          .dv
		.arb_fb_start           (fb_st_start),     //          .start

		.fb_st_start            (fb_st_start),      //     fb_st.start
		.fb_st_data             (fb_st_data),       //          .data
		.fb_st_dv               (fb_st_dv),         //          .dv
		.fb_st_ready            (fb_st_ready),       //          .ready
      
		.irq_in_port            (iSAM_INT),      //        irq.in_port
		.irq_out_port           (wIRQ_OUT)     //           .out_port
          

	);

assign oSAM_INT = wIRQ_OUT[1];

// NINA Bypass
                                            // SAM       NINA
assign oWM_RX     = bMKR_D[14];             // TX     => RX
assign bMKR_D[13] = iWM_TX;                 // RX     <= TX
assign oWM_RESET  = bMKR_D[7];                 // IO     => RESET
assign bWM_PIO27  = bMKR_D[6];              // IO     => BOOT
assign bMKR_D[10] = bWM_PIO1;               // MISO   => MOSI
assign bWM_PIO29  = bMKR_D[9];              // SCK    => SCK
assign bWM_PIO36  = bMKR_D[8];              // MOSI   => MISO
assign bWM_PIO28  = bMKR_D[5];              // CS     => CS
assign bMKR_D[4]  = bWM_PIO7;              // READY     => READY
  
endmodule
