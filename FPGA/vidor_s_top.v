module vidor_s_top
(
  // system signals
  input         iCLK,
  input         iRESETn,
  
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
  inout         bWM_PIO16,
  inout         bWM_PIO17,
  inout         bWM_PIO18,
  inout         bWM_PIO24,
  inout         bWM_PIO25,
  inout         bWM_PIO27,
  inout         bWM_PIO28,
  inout         bWM_PIO29,
  inout         bWM_PIO34,
  input         iWM_TX,
  output        oWM_RX,
  input         iWM_RTS,
  output        iWM_CTS,
  output        oWM_RESET,

  // HDMI output
  output [2:0]  oHDMI_TX,
  output        oHDMI_CLK,

  // MIPI input
  input  [1:0]  iMIPI_D,
  input         iMIPI_CLK,
  inout         bMIPI_SDA,
  inout         bMIPI_SCL,
  inout  [1:0]  bMIPI_GP,

  // misc pins
  output        panel_en,

  // Flash interface
  output        oFLASH_MOSI,
  input         iFLASH_MISO,
  output        oFLASH_SCK,
  output        oFLASH_CS
  
);

// signal declaration

wire        wOSC_CLK;

wire        wCLK8,wCLK24, wCLK64, wCLK120, wCLK100;

wire [31:0] wJTAG_ADDRESS, wJTAG_READ_DATA, wJTAG_WRITE_DATA, wDPRAM_READ_DATA;
wire        wJTAG_READ, wJTAG_WRITE, wJTAG_WAIT_REQUEST, wJTAG_READ_DATAVALID;
wire [4:0]  wJTAG_BURST_COUNT;
wire        wDPRAM_CS;

wire [7:0]  wTPG_RED,wTPG_GRN,wTPG_BLU;
wire        wTPG_HS,wTPG_VS,wTPG_DE;

wire        wVID_CLK, wVID_CLKx5;
wire        wMEM_CLK;

assign wVID_CLK   = wCLK24;
assign wVID_CLKx5 = wCLK120;
assign wMEM_CLK   = wCLK100;
assign wCLK8      = iCLK;

// NINA Bypass
                                            // SAM       NINA
assign oWM_RX     = bMKR_D[14];             // TX     => RX
assign bMKR_D[14] = iWM_TX;                 // RX     <= TX
assign oWM_RESET  = bMKR_D[7] ? 1'bZ : 0;   // IO     => RESET
assign bWM_PIO27  = bMKR_D[6];              // IO     => BOOT
assign bMKR_D[10] = bWM_PIO1;               // MISO   => MOSI
assign bWM_PIO28  = bMKR_D[9];              // SCK    => SCK
assign bWM_PIO21  = bMKR_D[8];              // MOSI   => MISO
assign bWM_PIO29  = bMKR_D[5];              // CS     => CS

// internal oscillator
cyclone10lp_oscillator   osc
	( 
	.clkout(wOSC_CLK),
	.oscena(1'b1));

// system PLL
SYSTEM_PLL (
	.areset(1'b0),
	.inclk0(wCLK8),
	.c0(wCLK24),
	.c1(wCLK120),
	.c2(wCLK100),
  //.c3(oSDRAM_CLK),
	.locked());

  // DVI output
DVI_OUT
(
  .iPCLK(wVID_CLK),
  .iSCLK(wVID_CLKx5),

  .iRED(wTPG_RED),
  .iGRN(wTPG_GRN),
  .iBLU(wTPG_BLU),
  .iHS(wTPG_HS),
  .iVS(wTPG_VS),
  .iDE(wTPG_DE),

  .oDVI_DATA(oHDMI_TX),
  .oDVI_CLK(oHDMI_CLK)
);

//audio generator

reg   [9:0] rAUDIO_ADDRESS;
wire [23:0] wAUDIO_DATA;
wire        wAUDIO_STROBE;

sinerom sine_inst(
	.address(rAUDIO_ADDRESS),
	.clock(wCLK24),
	.q(wAUDIO_DATA)
);

SD_MODULATOR #(
  .pBITS(24),
  .pINTERP_BITS(9)
) mod_inst
(
  .iCLK(wCLK24),
  .iDATA(wAUDIO_DATA),
  .oSTROBE(wAUDIO_STROBE),
  .oDAC(bMKR_A[0])
);
  
always @(posedge wCLK24)
begin
  if (wAUDIO_STROBE)
    rAUDIO_ADDRESS<=rAUDIO_ADDRESS+15;
  
end
/*
// frame reader
reg [21:0]  rFR_ADDR, rFR_RDADDR, rFR_WRADDR;
reg         rFR_READ, rFR_WRITE;
wire [15:0] wFR_DATA;
wire        wFR_DATAVALID;
wire        wFR_WAIT;
wire [7:0]  wFR_FIFO_UDW;
reg [7:0] rINITCNT;

reg [21:0] wFR_NEXT_RDADDR,wFR_NEXT_WRADDR;

  dcfifo #(
    .intended_device_family     ("CYCLONE 10 LP"),
    .lpm_type                   ("dcfifo"),
    .add_usedw_msb_bit          ("ON"),
    .overflow_checking          ("ON"),
    .rdsync_delaypipe           (4),
    .underflow_checking         ("ON"),
    .use_eab                    ("ON"),
    .write_aclr_synch           ("OFF"),
    .add_ram_output_register    ("OFF"),
    .lpm_numwords               (128),
    .lpm_showahead              ("ON"),
    .lpm_width                  (16),
    .lpm_widthu                 (8),
    .lpm_hint                   ("unused"),     
    .wrsync_delaypipe           (4) 
  )   
  DmaBufferfifo(
    .aclr         (1'b0),

    .wrclk        (wMEM_CLK),   
    .wrreq        (wFR_DATAVALID),              
    .wrempty      (),     
    .wrfull       (),               
    .wrusedw      (wFR_FIFO_UDW),
    .data         (wFR_DATA),   
 
    .rdclk        (wVID_CLK),     
    .rdreq        (wTPG_DE&&!wFR_RD_EMPTY),
    .rdempty      (wFR_RD_EMPTY),                                      
    .rdusedw      (),
    .rdfull       (),
    .q            ({wTPG_BLU[7:3],wTPG_GRN[7:3],wTPG_RED[7:3]}) 
  );


assign   wFR_NEXT_RDADDR = (rFR_READ&&!wFR_WAIT) ? (rFR_RDADDR==640*480-1) ? 0 : rFR_RDADDR+1 : rFR_RDADDR;
assign   wFR_NEXT_WRADDR = (rFR_WRITE&&!wFR_WAIT) ?(rFR_WRADDR==640*480-1) ? 0 : rFR_WRADDR+1 : rFR_WRADDR;

always @(posedge wMEM_CLK)
begin
  if (rINITCNT[7]) begin
    if (rFR_READ&&!wFR_WAIT)
    begin
      rFR_READ <=0;
      rFR_RDADDR<= wFR_NEXT_RDADDR;
    end
    if (rFR_WRITE&&!wFR_WAIT)
    begin
      rFR_WRITE <=0;
      rFR_WRADDR<= wFR_NEXT_WRADDR;
    end
    
    if ((wFR_FIFO_UDW<64))
    begin
      rFR_WRITE<=0;
      rFR_READ <=1;
      rFR_ADDR <= wFR_NEXT_RDADDR;
    end
    else begin
      rFR_READ  <=0;
      rFR_WRITE <=1;
      rFR_ADDR  <= wFR_NEXT_WRADDR;
    end
  end else begin
    rINITCNT <=rINITCNT+1;
  end 
end

// SDRAM controller

  assign oSDRAM_CLK=wMEM_CLK;
  /*
	memory_sdram sdram (
		.clk            (wMEM_CLK),                         //   clk.clk
		.reset_n        (rINITCNT[7]|rINITCNT[6]), // reset.reset_n
		.az_addr        (rFR_ADDR),                //    s1.address
		.az_be_n        (2'b0),           //      .byteenable_n
		.az_cs          (1'b1),             //      .chipselect
		.az_data        (rFR_WRADDR),              //      .writedata
		.az_rd_n        (!rFR_READ),                 //      .read_n
		.az_wr_n        (!rFR_WRITE),                //      .write_n
		.za_data        (wFR_DATA),               //      .readdata
		.za_valid       (wFR_DATAVALID),          //      .readdatavalid
		.za_waitrequest (wFR_WAIT),            //      .waitrequest

		.zs_addr        (oSDRAM_ADDR),                      //  wire.export
		.zs_ba          (oSDRAM_BA),                        //      .export
		.zs_cas_n       (oSDRAM_CASn),                     //      .export
		.zs_cke         (oSDRAM_CKE),                       //      .export
		.zs_cs_n        (oSDRAM_CSn),                      //      .export
		.zs_dq          (bSDRAM_DQ),                        //      .export
		.zs_dqm         (oSDRAM_DQM),                       //      .export
		.zs_ras_n       (oSDRAM_RASn),                     //      .export
		.zs_we_n        (oSDRAM_WEn)                       //      .export
	);
  */
assign oSDRAM_CLK=wMEM_CLK;
memory u0(
		.clk_clk                (wMEM_CLK),               //      clk.clk
		.sdram_addr             (oSDRAM_ADDR), //    sdram.addr
		.sdram_ba               (oSDRAM_BA),   //         .ba
		.sdram_cas_n            (oSDRAM_CASn), //         .cas_n
		.sdram_cke              (oSDRAM_CKE),  //         .cke
		.sdram_cs_n             (oSDRAM_CSn),  //         .cs_n
		.sdram_dq               (bSDRAM_DQ),   //         .dq
		.sdram_dqm              (oSDRAM_DQM),  //         .dqm
		.sdram_ras_n            (oSDRAM_RASn), //         .ras_n
		.sdram_we_n             (oSDRAM_WEn),   //         .we_n
 		.vport_blu              (wTPG_BLU),     // vport.blu
		.vport_de               (wTPG_DE),      //      .de
		.vport_grn              (wTPG_GRN),     //      .grn
		.vport_hs               (wTPG_HS),      //      .hs
		.vport_vs               (wTPG_VS),      //      .vs
		.vport_red              (wTPG_RED),     //      .red
		.reset_reset_n          (1'b1), // reset.reset_n
		.vid_clk                (wVID_CLK),        //   vid.clk
		.flash_spi_MISO         (iFLASH_MISO),   // flash_spi.MISO
		.flash_spi_MOSI         (oFLASH_MOSI),   //          .MOSI
		.flash_spi_SCLK         (oFLASH_SCK),   //          .SCLK
		.flash_spi_SS_n         (oFLASH_CS),   //          .SS_n
		.csi_i2c_scl            (bMIPI_SCL),      //   csi_i2c.scl
		.csi_i2c_sda            (bMIPI_SDA),      //          .sda
		.dpram_address          (wJTAG_ADDRESS[7:0]),    //     dpram.address
		.dpram_chipselect       (wDPRAM_CS), //          .chipselect
		.dpram_clken            (1'b1),      //          .clken
		.dpram_write            (wJTAG_WRITE),      //          .write
		.dpram_readdata         (wDPRAM_READ_DATA),   //          .readdata
		.dpram_writedata        (wJTAG_WRITE_DATA),  //          .writedata
		.dpram_byteenable       (4'b1111)  //          .byteenable

	);
  

// Quadrature DECODER


parameter pENCODERS=2;
wire [pENCODERS-1:0] wENCODER_A,wENCODER_B;

assign wENCODER_A[0] = bMKR_A[1];
assign wENCODER_B[0] = bMKR_A[2];

assign wENCODER_A[1] = bMKR_A[5];
assign wENCODER_B[1] = bMKR_A[6];

reg [3:0][pENCODERS-1:0] rRESYNC_ENCODER_A,rRESYNC_ENCODER_B;
reg [5:0] rENC_PRESCALER;
reg [15:0][pENCODERS-1:0] rSTEPS;
wire [pENCODERS-1:0] wINCREMENT =  rRESYNC_ENCODER_A[2]&!rRESYNC_ENCODER_A[3]& rRESYNC_ENCODER_B[3] | !rRESYNC_ENCODER_A[2]& rRESYNC_ENCODER_A[3]& !rRESYNC_ENCODER_B[3];
wire [pENCODERS-1:0] wDECREMENT = !rRESYNC_ENCODER_A[2]& rRESYNC_ENCODER_A[3]& rRESYNC_ENCODER_B[3] |  rRESYNC_ENCODER_A[2]&!rRESYNC_ENCODER_A[3]& !rRESYNC_ENCODER_B[3];
integer i;

always @(posedge iCLK)
begin
  rENC_PRESCALER<= rENC_PRESCALER+1;
  if (rENC_PRESCALER==0)
  begin
    rRESYNC_ENCODER_A<={rRESYNC_ENCODER_A,wENCODER_A};
    rRESYNC_ENCODER_B<={rRESYNC_ENCODER_B,wENCODER_B};
    for (i=0;i<pENCODERS;i++)
    begin
      if (wINCREMENT[i])
        rSTEPS[i] <= rSTEPS[i]+1;
      if (wDECREMENT[i])
        rSTEPS[i] <= rSTEPS[i]-1;
    end
  end
end

// JTAG Bridge

assign wDPRAM_CS=wJTAG_ADDRESS[8];

JTAG_BRIDGE (
  .iCLK(wOSC_CLK),
  .iRESET(1'b0),
  .oADDRESS(wJTAG_ADDRESS),
  .oWRITE(wJTAG_WRITE),
  .oREAD(wJTAG_READ),
  .oWRITE_DATA(wJTAG_WRITE_DATA),
  .iREAD_DATA(wJTAG_READ_DATA),
  .oBURST_COUNT(wJTAG_BURST_COUNT),
  .iWAIT_REQUEST(wJTAG_WAIT_REQUEST),
  .iREAD_DATAVALID(wJTAG_READ_DATAVALID)
);

reg rJTAG_DATAVALID;
reg [31:0] rJTAG_READ_DATA;
assign wJTAG_READ_DATA = rCLIENTID ? wDPRAM_READ_DATA : rJTAG_READ_DATA;
assign wJTAG_READ_DATAVALID = rJTAG_DATAVALID;
reg rCLIENTID;

always @(posedge wOSC_CLK)
begin
  rJTAG_DATAVALID<=wJTAG_READ;
  case (wJTAG_ADDRESS)
    0: rJTAG_READ_DATA<= 32'hbac1beef;
    1: rJTAG_READ_DATA<= 32'haaaa5555;
    2: rJTAG_READ_DATA<= 32'h5555aaaa;
    3: rJTAG_READ_DATA<= 32'h01234567;
    4: rJTAG_READ_DATA<= rSTEPS[0];
    5: rJTAG_READ_DATA<= rSTEPS[1];
  endcase;
  if (wJTAG_READ)
  begin
    rCLIENTID <= wDPRAM_CS;
  end
end

endmodule
