module MKRVIDOR4000_peripherals_top
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
/*
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
*/
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

//wire [7:0]  wDVI_RED,wDVI_GRN,wDVI_BLU;
//wire        wDVI_HS, wDVI_VS, wDVI_DE;

//wire        wVID_CLK, wVID_CLKx5;
wire        wMEM_CLK;

//assign wVID_CLK   = wCLK24;
//assign wVID_CLKx5 = wCLK120;
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


//wire fb_st_start,fb_st_dv,fb_st_ready;
//wire [30:0] fb_st_data;

//wire [23:0] mipi_st_data;
//wire mipi_st_start,mipi_st_dv;

//wire [23:0] mipi_ste_data;
//wire mipi_ste_start,mipi_ste_dv;

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

//wire [1:0] wIRQ_OUT;
wire [4:0] wNPIX;

wire wI2C0_SCL_I,wI2C0_SCL_O,wI2C0_SCL_E,wI2C0_SDA_I,
     wI2C0_SDA_O,wI2C0_SDA_E,wI2C1_SCL_I,wI2C1_SCL_O,
     wI2C1_SCL_E,wI2C1_SDA_I,wI2C1_SDA_O,wI2C1_SDA_E,
     wI2C2_SCL_I,wI2C2_SCL_O,wI2C2_SCL_E,wI2C2_SDA_I,
     wI2C2_SDA_O,wI2C2_SDA_E,wI2C3_SCL_I,wI2C3_SCL_O,
     wI2C3_SCL_E,wI2C3_SDA_I,wI2C3_SDA_O,wI2C3_SDA_E;

wire wUART0_CTS,wUART0_RTS,wUART0_DSR,wUART0_DCD,wUART0_DTR,wUART0_RX,wUART0_TX,
     wUART1_CTS,wUART1_RTS,wUART1_DSR,wUART1_DCD,wUART1_DTR,wUART1_RX,wUART1_TX,
     wUART2_CTS,wUART2_RTS,wUART2_DSR,wUART2_DCD,wUART2_DTR,wUART2_RX,wUART2_TX,
     wUART3_CTS,wUART3_RTS,wUART3_DSR,wUART3_DCD,wUART3_DTR,wUART3_RX,wUART3_TX,
     wUART4_CTS,wUART4_RTS,wUART4_DSR,wUART4_DCD,wUART4_DTR,wUART4_RX,wUART4_TX,
     wUART5_CTS,wUART5_RTS,wUART5_DSR,wUART5_DCD,wUART5_DTR,wUART5_RX,wUART5_TX,
     wUART6_CTS,wUART6_RTS,wUART6_DSR,wUART6_DCD,wUART6_DTR,wUART6_RX,wUART6_TX,
     wUART7_CTS,wUART7_RTS,wUART7_DSR,wUART7_DCD,wUART7_DTR,wUART7_RX,wUART7_TX;

wire wSPI0_MOSI,wSPI0_SCK,wSPI0_MISO,wSPI0_CS,
     wSPI1_MOSI,wSPI1_SCK,wSPI1_MISO,wSPI1_CS,
     wSPI2_MOSI,wSPI2_SCK,wSPI2_MISO,wSPI2_CS,
     wSPI3_MOSI,wSPI3_SCK,wSPI3_MISO,wSPI3_CS,
     wSPI4_MOSI,wSPI4_SCK,wSPI4_MISO,wSPI4_CS;

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

MKRVIDOR4000_peripherals_sys u0(
    .clk_clk                (wMEM_CLK),       //      clk.clk
    .reset_reset_n          (rRESETCNT[5]),   // reset.reset_n
//    .vid_clk                (wVID_CLK),       //   vid.clk
    .clk_0_clk              (wFLASH_CLK),
    .reset_0_reset_n        (rRESETCNT[5]),   // reset.reset_n

    .sdram_addr             (oSDRAM_ADDR),    //    sdram.addr
    .sdram_ba               (oSDRAM_BA),      //         .ba
    .sdram_cas_n            (oSDRAM_CASn),    //         .cas_n
    .sdram_cke              (oSDRAM_CKE),     //         .cke
    .sdram_cs_n             (oSDRAM_CSn),     //         .cs_n
    .sdram_dq               (bSDRAM_DQ),      //         .dq
    .sdram_dqm              (oSDRAM_DQM),     //         .dqm
    .sdram_ras_n            (oSDRAM_RASn),    //         .ras_n
    .sdram_we_n             (oSDRAM_WEn),     //         .we_n

    .flash_spi_MISO         (iFLASH_MISO ),   // flash_spi.MISO
    .flash_spi_MOSI         (wFLASH_MOSI),    //          .MOSI
    .flash_spi_SCLK         (wFLASH_SCK),     //          .SCLK
    .flash_spi_CS           (wFLASH_CS),      //          .SS_n

    .qspi_dclk              (wQSPI_CLK),      //      qspi.dclk
    .qspi_ncs               (wQSPI_NCS),      //          .ncs
    .qspi_oe                (wQSPI_OE),       //          .oe
    .qspi_dataout           (wQSPI_DATAOUT),  //          .dataout
    .qspi_dataoe            (wQSPI_DATAOE),   //          .dataoe
    .qspi_datain            ({oFLASH_HOLD, oFLASH_WP, iFLASH_MISO, oFLASH_MOSI}),      //          .datain

    .nina_spi_MISO          (wNINA_MISO),     // flash_spi.MISO
    .nina_spi_MOSI          (wNINA_MOSI),     //          .MOSI
    .nina_spi_SCLK          (wNINA_SCLK),     //          .SCLK
    .nina_spi_CS            (wNINA_SS),       //          .SS_n

    .nina_uart_sin          (wWM_PIO_IN[16]), //  nina_uart.sin
    .nina_uart_sout         (wNINA_RX),       //           .sout
    //.nina_uart_sout_oe, //           .sout_oe

    .sam_pio_in             (wSAM_PIO_IN),       //   sam_pio.in
    .sam_pio_out            (wSAM_PIO_OUT),      //          .out
    .sam_pio_dir            (wSAM_PIO_DIR),      //          .dir
    .sam_pio_msel           (wSAM_PIO_MSEL),     //          .msel

    .wm_pio_in              (wWM_PIO_IN),        //    wm_pio.in
    .wm_pio_out             (wWM_PIO_OUT),       //          .out
    .wm_pio_dir             (wWM_PIO_DIR),       //          .dir
    .wm_pio_msel            (wWM_PIO_MSEL),      //          .msel

    .pex_pio_in             (wPEX_PIO_IN),       //   pex_pio.in
    .pex_pio_out            (wPEX_PIO_OUT),      //          .out
    .pex_pio_dir            (wPEX_PIO_DIR),      //          .dir
    .pex_pio_msel           (wPEX_PIO_MSEL),     //          .msel

    .sam_pwm_pwm            (wSAM_OUT1),

    .encoder_encoder_a      ({bMKR_D[13],bMKR_D[11],bMKR_D[9],bMKR_D[7],bMKR_D[5],bMKR_D[3],bMKR_D[1],bMKR_A[6],bMKR_A[4],bMKR_A[2],bMKR_A[0]}), //    encoder.encoder_a
    .encoder_encoder_b      ({bMKR_D[14],bMKR_D[12],bMKR_D[10],bMKR_D[8],bMKR_D[6],bMKR_D[4],bMKR_D[2],bMKR_D[0],bMKR_A[5],bMKR_A[3],bMKR_A[1]}),		//           .encoder_b

    .uart0_ctl_cts_n   (wUART0_CTS),   // uart0_ctl.cts_n
    .uart0_ctl_rts_n   (wUART0_RTS),   //          .rts_n
    .uart0_ctl_dsr_n   (wUART0_DSR),   //          .dsr_n
    .uart0_ctl_dcd_n   (wUART0_DCD),   //          .dcd_n
    .uart0_ctl_dtr_n   (wUART0_DTR),   //          .dtr_n
    .uart0_sin         (wUART0_RX),    //     uart1.sin
    .uart0_sout        (wUART0_TX),    //          .sout

    .uart1_ctl_cts_n   (wUART1_CTS),   // uart0_ctl.cts_n
    .uart1_ctl_rts_n   (wUART1_RTS),   //          .rts_n
    .uart1_ctl_dsr_n   (wUART1_DSR),   //          .dsr_n
    .uart1_ctl_dcd_n   (wUART1_DCD),   //          .dcd_n
    .uart1_ctl_dtr_n   (wUART1_DTR),   //          .dtr_n
    .uart1_sin         (wUART1_RX),    //     uart1.sin
    .uart1_sout        (wUART1_TX),    //          .sout

    .uart2_ctl_cts_n   (wUART2_CTS),   // uart0_ctl.cts_n
    .uart2_ctl_rts_n   (wUART2_RTS),   //          .rts_n
    .uart2_ctl_dsr_n   (wUART2_DSR),   //          .dsr_n
    .uart2_ctl_dcd_n   (wUART2_DCD),   //          .dcd_n
    .uart2_ctl_dtr_n   (wUART2_DTR),   //          .dtr_n
    .uart2_sin         (wUART2_RX),    //     uart1.sin
    .uart2_sout        (wUART2_TX),    //          .sout

    .uart3_ctl_cts_n   (wUART3_CTS),   // uart0_ctl.cts_n
    .uart3_ctl_rts_n   (wUART3_RTS),   //          .rts_n
    .uart3_ctl_dsr_n   (wUART3_DSR),   //          .dsr_n
    .uart3_ctl_dcd_n   (wUART3_DCD),   //          .dcd_n
    .uart3_ctl_dtr_n   (wUART3_DTR),   //          .dtr_n
    .uart3_sin         (wUART3_RX),    //     uart1.sin
    .uart3_sout        (wUART3_TX),    //          .sout

    .uart4_ctl_cts_n   (wUART4_CTS),   // uart0_ctl.cts_n
    .uart4_ctl_rts_n   (wUART4_RTS),   //          .rts_n
    .uart4_ctl_dsr_n   (wUART4_DSR),   //          .dsr_n
    .uart4_ctl_dcd_n   (wUART4_DCD),   //          .dcd_n
    .uart4_ctl_dtr_n   (wUART4_DTR),   //          .dtr_n
    .uart4_sin         (wUART4_RX),    //     uart1.sin
    .uart4_sout        (wUART4_TX),    //          .sout

    .uart5_ctl_cts_n   (wUART5_CTS),   // uart0_ctl.cts_n
    .uart5_ctl_rts_n   (wUART5_RTS),   //          .rts_n
    .uart5_ctl_dsr_n   (wUART5_DSR),   //          .dsr_n
    .uart5_ctl_dcd_n   (wUART5_DCD),   //          .dcd_n
    .uart5_ctl_dtr_n   (wUART5_DTR),   //          .dtr_n
    .uart5_sin         (wUART5_RX),    //     uart1.sin
    .uart5_sout        (wUART5_TX),    //          .sout

    .uart6_ctl_cts_n   (wUART6_CTS),   // uart0_ctl.cts_n
    .uart6_ctl_rts_n   (wUART6_RTS),   //          .rts_n
    .uart6_ctl_dsr_n   (wUART6_DSR),   //          .dsr_n
    .uart6_ctl_dcd_n   (wUART6_DCD),   //          .dcd_n
    .uart6_ctl_dtr_n   (wUART6_DTR),   //          .dtr_n
    .uart6_sin         (wUART6_RX),    //     uart1.sin
    .uart6_sout        (wUART6_TX),    //          .sout

    .uart7_ctl_cts_n   (wUART7_CTS),   // uart0_ctl.cts_n
    .uart7_ctl_rts_n   (wUART7_RTS),   //          .rts_n
    .uart7_ctl_dsr_n   (wUART7_DSR),   //          .dsr_n
    .uart7_ctl_dcd_n   (wUART7_DCD),   //          .dcd_n
    .uart7_ctl_dtr_n   (wUART7_DTR),   //          .dtr_n
    .uart7_sin         (wUART7_RX),    //     uart1.sin
    .uart7_sout        (wUART7_TX),    //          .sout

    .i2c0_scl_i        (wI2C0_SCL_I),         //      i2c0.scl_i
    .i2c0_scl_o        (wI2C0_SCL_O),         //          .scl_o
    .i2c0_scl_en       (wI2C0_SCL_E),         //          .scl_en
    .i2c0_sda_i        (wI2C0_SDA_I),         //          .sda_i
    .i2c0_sda_o        (wI2C0_SDA_O),         //          .sda_o
    .i2c0_sda_en       (wI2C0_SDA_E),         //          .sda_en
    .i2c1_scl_i        (wI2C1_SCL_I),         //      i2c1.scl_i
    .i2c1_scl_o        (wI2C1_SCL_O),         //          .scl_o
    .i2c1_scl_en       (wI2C1_SCL_E),         //          .scl_en
    .i2c1_sda_i        (wI2C1_SDAL_I),        //          .sda_i
    .i2c1_sda_o        (wI2C1_SDAL_O),        //          .sda_o
    .i2c1_sda_en       (wI2C1_SDAL_E),        //          .sda_en
    .i2c2_scl_i        (wI2C2_SCL_I),         //      i2c2.scl_i
    .i2c2_scl_o        (wI2C2_SCL_O),         //          .scl_o
    .i2c2_scl_en       (wI2C2_SCL_E),         //          .scl_en
    .i2c2_sda_i        (wI2C2_SDA_I),         //          .sda_i
    .i2c2_sda_o        (wI2C2_SDA_O),         //          .sda_o
    .i2c2_sda_en       (wI2C2_SDA_E),         //          .sda_en
    .i2c3_scl_i        (wI2C3_SCL_I),         //      i2c3.scl_i
    .i2c3_scl_o        (wI2C3_SCL_O),         //          .scl_o
    .i2c3_scl_en       (wI2C3_SCL_E),         //          .scl_en
    .i2c3_sda_i        (wI2C3_SDA_I),         //          .sda_i
    .i2c3_sda_o        (wI2C3_SDA_O),         //          .sda_o
    .i2c3_sda_en       (wI2C3_SDA_E),         //          .sda_en

    .spi0_MOSI         (wSPI0_MOSI),          //      spi0.MOSI
    .spi0_SCLK         (wSPI0_SCK),           //          .SCLK
    .spi0_MISO         (wSPI0_MISO),          //          .MISO
    .spi0_CS           (wSPI0_CS),            //          .CS
    .spi1_MOSI         (wSPI1_MOSI),          //      spi1.MOSI
    .spi1_SCLK         (wSPI1_SCK),           //          .SCLK
    .spi1_MISO         (wSPI1_MISO),          //          .MISO
    .spi1_CS           (wSPI1_CS),            //          .CS
    .spi2_MOSI         (wSPI2_MOSI),          //      spi2.MOSI
    .spi2_SCLK         (wSPI2_SCK),           //          .SCLK
    .spi2_MISO         (wSPI2_MISO),          //          .MISO
    .spi2_CS           (wSPI2_CS),            //          .CS
    .spi3_MOSI         (wSPI3_MOSI),          //      spi3.MOSI
    .spi3_SCLK         (wSPI3_SCK),           //          .SCLK
    .spi3_MISO         (wSPI3_MISO),          //          .MISO
    .spi3_CS           (wSPI3_CS),            //          .CS
    .spi4_MOSI         (wSPI4_MOSI),          //      spi4.MOSI
    .spi4_SCLK         (wSPI4_SCK),           //          .SCLK
    .spi4_MISO         (wSPI4_MISO),          //          .MISO
    .spi4_CS           (wSPI4_CS),            //          .CS

    .mb_rq             (iSAM_INT),            //        iMST_RQ
    .mb_ak             (oSAM_INT),            //        oMST_AK

    .pixel_data        (wNPIX)                //     pixel.data

  );
//assign oSAM_INT = wIRQ_OUT[1];

assign wWM_OUT2[11]     = wNINA_SS;
assign wWM_OUT2[18]     = wNINA_SCLK;
assign wWM_OUT2[19]     = wNINA_MOSI;
assign wWM_OUT2[15]     = wNINA_RX;

assign wWM_OUT1[15]     = wSAM_PIO_IN[22]; // D14 for NINA_RX in bypass
assign wWM_OUT1[0]      = wSAM_PIO_IN[15]; // D7 for NINA_RESET in bypass
assign wWM_OUT1[10]     = wSAM_PIO_IN[14]; // D6 for NINA_GPIO0 in bypass

assign wNINA_MISO       = wWM_PIO_IN[20]; // route NINA_MISO to internal SPI

wire [31:0] wSAM_PIN_OUT,wSAM_OUT1,wSAM_OUT2,wSAM_OUT3,wSAM_OEN2, wSAM_OEN3;
wire [31:0] wWM_PIN_OUT,wWM_OUT1,wWM_OUT2,wWM_OUT3;
wire [31:0] wPEX_PIN_OUT,wPEX_OUT1,wPEX_OUT2,wPEX_OUT3;

assign wSAM_PIO_IN = {bMKR_D,bMKR_A,bMKR_AREF};
assign wWM_PIO_IN = {bWM_PIO1,bWM_PIO36,bWM_PIO29,iWM_PIO32,iWM_TX,oWM_RX,bWM_PIO35,bWM_PIO34,bWM_PIO31,bWM_PIO28,bWM_PIO27,bWM_PIO21,bWM_PIO20,bWM_PIO18,bWM_PIO8,bWM_PIO7,bWM_PIO5,bWM_PIO4,bWM_PIO3,bWM_PIO2,oWM_RESET};
assign wPEX_PIO_IN = {iPEX_PIN33,iPEX_PIN31,iPEX_PIN25,iPEX_PIN23,iPEX_PIN13,iPEX_PIN11,bPEX_PIN51,bPEX_PIN49,bPEX_PIN48,bPEX_PIN47,bPEX_PIN46,bPEX_PIN45,bPEX_PIN44,bPEX_PIN42,bPEX_PIN32,bPEX_PIN30,bPEX_PIN28,bPEX_PIN20,bPEX_PIN16,bPEX_PIN14,bPEX_PIN12,bPEX_PIN10,bPEX_PIN8,bPEX_PIN6,bPEX_RST};

assign {bMKR_D,bMKR_A,bMKR_AREF}= wSAM_PIN_OUT;
assign {bWM_PIO1,bWM_PIO36,bWM_PIO29,wDUMMY1,wDUMMY0,oWM_RX,bWM_PIO35,bWM_PIO34,bWM_PIO31,bWM_PIO28,bWM_PIO27,bWM_PIO21,bWM_PIO20,bWM_PIO18,bWM_PIO8,bWM_PIO7,bWM_PIO5,bWM_PIO4,bWM_PIO3,bWM_PIO2,oWM_RESET} = wWM_PIN_OUT;
assign {bPEX_PIN51,bPEX_PIN49,bPEX_PIN48,bPEX_PIN47,bPEX_PIN46,bPEX_PIN45,bPEX_PIN44,bPEX_PIN42,bPEX_PIN32,bPEX_PIN30,bPEX_PIN28,bPEX_PIN20,bPEX_PIN16,bPEX_PIN14,bPEX_PIN12,bPEX_PIN10,bPEX_PIN8,bPEX_PIN6,bPEX_RST} = wPEX_PIN_OUT;

/*assign wSAM_OEN2[23:18] = {
                            wI2C2_SDA_E, wI2C2_SCL_E,
                            wI2C1_SDA_E, wI2C1_SCL_E,
                            wI2C0_SDA_E, wI2C0_SCL_E };*/
assign wSAM_OEN2 = { {8{1'b1}},
                            wI2C2_SDA_E, wI2C2_SCL_E,
                            wI2C1_SDA_E, wI2C1_SCL_E,
                            wI2C0_SDA_E, wI2C0_SCL_E,
                            {17{1'b1}},
                            1'b0 };


assign {
      wI2C2_SDA_I,wI2C2_SCL_I,
      wI2C1_SDA_I,wI2C1_SCL_I,
      wI2C0_SDA_I,wI2C0_SCL_I,
      wUART7_RX,wUART1_DSR,
      wUART6_RX,wUART7_CTS,
      wUART5_RX,wUART7_DSR,
      wUART4_RX,wUART5_CTS,
      wUART3_RX,wUART5_DSR,
      wUART2_RX,wUART3_CTS,
      wUART1_RX,wUART3_DSR,
      wUART0_RX,wUART1_CTS} = wSAM_PIO_IN[23:1];

assign wSAM_OUT2[23:1]={
            wI2C2_SDA_O,wI2C2_SCL_O,
            wI2C1_SDA_O,wI2C1_SCL_O,
            wI2C0_SDA_O,wI2C0_SCL_O,
            wUART1_DTR,wUART7_TX,
            wUART7_RTS,wUART6_TX,
            wUART7_DTR,wUART5_TX,
            wUART5_RTS,wUART4_TX,
            wUART5_DTR,wUART3_TX,
            wUART3_RTS,wUART2_TX,
            wUART3_DTR,wUART1_TX,
            wUART1_RTS,wUART0_TX,
            };

/* assign wSAM_OEN3[2:1] = { wI2C3_SDA_E, wI2C3_SCL_E}; */
assign wSAM_OEN3={ {29{1'b1}}, wI2C3_SDA_E, wI2C3_SCL_E,1'b0};

assign {wI2C3_SDA_I,wI2C3_SCL_I} = wSAM_PIO_IN[2:1];
assign {wSPI1_MISO, wSPI0_MISO} = wSAM_PIO_IN[8:7];
assign {wSPI3_MISO, wSPI2_MISO} = wSAM_PIO_IN[16:15];
assign wSPI4_MISO = wSAM_PIO_IN[20];
assign wSAM_OUT3[23:1] = {wSPI4_CS,wSPI4_SCK,wNPIX[4],wSPI4_MOSI,
                          wSPI3_CS,wSPI3_SCK,wNPIX[3],wNPIX[2],
                          wSPI3_MOSI,wSPI2_CS,wSPI2_SCK, wSPI2_MOSI,
                          wSPI1_CS,wSPI1_SCK,wNPIX[1],wNPIX[0],
                          wSPI1_MOSI,wSPI0_CS,wSPI0_SCK,wSPI0_MOSI,
                          wI2C3_SDA_O, wI2C3_SCL_O};

genvar i;
generate

for (i=0;i<31;i++) begin : genloop
assign wSAM_PIN_OUT[i] =  (wSAM_PIO_MSEL[i*2+1-:2] ==0) ?
                            (i==18 && wWM_PIO_MSEL[20*2+1-:2]==1) ? wWM_PIO_IN[20] : // NINA_MISO to SAM (D10) in bypass
                            (i==21 && wWM_PIO_MSEL[15*2+1-:2]==1) ? wWM_PIO_IN[15] : // NINA_TX to SAM (D13) in bypass
                            (i==12 && wWM_PIO_MSEL[5*2+1-:2]==1) ? wWM_PIO_IN[5] : // ACK from NINA to SAM (D4) in bypass
                            !wSAM_PIO_DIR[i] ? 1'bZ :wSAM_PIO_OUT[i] :
                          (wSAM_PIO_MSEL[i*2+1-:2] ==1) ? wSAM_OUT1[i] :
                          (wSAM_PIO_MSEL[i*2+1-:2] ==2) ? wSAM_OEN2 ? wSAM_OUT2[i] : 1'bz :
                          (wSAM_PIO_MSEL[i*2+1-:2] ==3) ? wSAM_OEN3 ? wSAM_OUT3[i] : 1'bz :0;
/*                          (wSAM_PIO_MSEL[i*2+1-:2] ==3) ? wSAM_OUT3[i] :0;*/
assign wWM_PIN_OUT[i] =
                          (wWM_PIO_MSEL[i*2+1-:2] ==0) ? !wWM_PIO_DIR[i] ? 1'bZ : wWM_PIO_OUT[i] :
                          (wWM_PIO_MSEL[i*2+1-:2] ==1) ? wWM_OUT1[i] :
                          (wWM_PIO_MSEL[i*2+1-:2] ==2) ? wWM_OUT2[i] :
                          (wWM_PIO_MSEL[i*2+1-:2] ==3) ? wWM_OUT3[i] : 0;
assign wPEX_PIN_OUT[i] =
                          (wPEX_PIO_MSEL[i*2+1-:2] ==0) ? !wPEX_PIO_DIR[i] ? 1'bZ : wPEX_PIO_OUT[i] :
                          (wPEX_PIO_MSEL[i*2+1-:2] ==1) ? wPEX_OUT1[i] :
                          (wPEX_PIO_MSEL[i*2+1-:2] ==2) ? wPEX_OUT2[i] :
                          (wPEX_PIO_MSEL[i*2+1-:2] ==3) ? wPEX_OUT3[i] : 0;

end
endgenerate

endmodule
