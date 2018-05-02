module MIPI_RX
(
  input  [1:0]  iMIPI_D,
  input         iMIPI_CLK,

  input         iMEM_CLK,
  input         iRESET,
  output [31:0] oADDRESS,
  output [15:0] oDATA,
  output        oWRITE,
  input         iWAITREQUEST
);


wire [1:0] wMIPI_DH;
wire [1:0] wMIPI_DL;
	altddio_in	ALTDDIO_IN_component (
				.datain (iMIPI_D),
				.inclock (iMIPI_CLK),
				.dataout_h (wMIPI_DH),
				.dataout_l (wMIPI_DL),
				.aclr (1'b0),
				.aset (1'b0),
				.inclocken (1'b1),
				.sclr (1'b0),
				.sset (1'b0));
	defparam
		ALTDDIO_IN_component.intended_device_family = "Cyclone 10 LP",
		ALTDDIO_IN_component.invert_input_clocks = "OFF",
		ALTDDIO_IN_component.lpm_hint = "UNUSED",
		ALTDDIO_IN_component.lpm_type = "altddio_in",
		ALTDDIO_IN_component.power_up_high = "OFF",
		ALTDDIO_IN_component.width = 2;


reg [2:0] rMIPI_ZC /* synthesis noprune */;
reg [11:0] rMIPI_D0,rMIPI_D1 /* synthesis noprune */;
reg rMIPI_HDR, rMIPI_HDR2/* synthesis noprune */;
reg rMIPI_VSTART/* synthesis noprune */;
reg rMIPI_VDATA/* synthesis noprune */;
reg rMIPI_BAYERPOL /* synthesis noprune */;
reg [15:0] rMIPI_LINEMEM [639:0];
reg rMIPI_PIXELVALID/* synthesis noprune */;
reg rMIPI_PIXELVALID2/* synthesis noprune */;
reg rMIPI_DATAVALID/* synthesis noprune */;
reg [7:0] rMIPI_R/* synthesis noprune */;
reg [7:0] rMIPI_G0 /* synthesis noprune */;
reg [7:0] rMIPI_B /* synthesis noprune */;
reg [7:0] rMIPI_G1 /* synthesis noprune */;
localparam cMIPI_HT_VSTART = 0;
localparam cMIPI_HT_VEND = 1;
localparam cMIPI_HT_HSTART = 2;
localparam cMIPI_HT_HEND = 3;
localparam cMIPI_HT_RAW8 = 42;
reg [2:0] rPRESCALE /* synthesis noprune */;
reg [15:0] rWC      /* synthesis noprune */;
reg rPHASE /* synthesis noprune */;
wire [15:0] wMIPI_DATA      /* synthesis noprune */;
wire [15:0] wMIPI_LB_DATA;


	altsyncram	altsyncram_component (
				.address_a (rWC),
				.address_b (rWC),
				.clock0 (iMIPI_CLK),
				.data_a (wMIPI_DATA),
				.wren_a (rMIPI_DATAVALID&rMIPI_VDATA),
				.q_b (wMIPI_LB_DATA),
				.aclr0 (1'b0),
				.aclr1 (1'b0),
				.addressstall_a (1'b0),
				.addressstall_b (1'b0),
				.byteena_a (1'b1),
				.byteena_b (1'b1),
				.clock1 (1'b1),
				.clocken0 (1'b1),
				.clocken1 (1'b1),
				.clocken2 (1'b1),
				.clocken3 (1'b1),
				.data_b ({16{1'b1}}),
				.eccstatus (),
				.q_a (),
				.rden_a (1'b1),
				.rden_b (1'b1),
				.wren_b (1'b0));
	defparam
		altsyncram_component.address_aclr_b = "NONE",
		altsyncram_component.address_reg_b = "CLOCK0",
		altsyncram_component.clock_enable_input_a = "BYPASS",
		altsyncram_component.clock_enable_input_b = "BYPASS",
		altsyncram_component.clock_enable_output_b = "BYPASS",
		altsyncram_component.intended_device_family = "Cyclone 10 LP",
		altsyncram_component.lpm_type = "altsyncram",
		altsyncram_component.numwords_a = 1024,
		altsyncram_component.numwords_b = 1024,
		altsyncram_component.operation_mode = "DUAL_PORT",
		altsyncram_component.outdata_aclr_b = "NONE",
		altsyncram_component.outdata_reg_b = "UNREGISTERED",
		altsyncram_component.power_up_uninitialized = "FALSE",
		altsyncram_component.read_during_write_mode_mixed_ports = "DONT_CARE",
		altsyncram_component.widthad_a = 10,
		altsyncram_component.widthad_b = 10,
		altsyncram_component.width_a = 16,
		altsyncram_component.width_b = 16,
		altsyncram_component.width_byteena_a = 1;

    
assign wMIPI_DATA = {rMIPI_D1[7:0],rMIPI_D0[7:0]};

always @(posedge iMIPI_CLK)
begin
  rMIPI_D0<= rPHASE ? {wMIPI_DH[0],wMIPI_DL[0],rMIPI_D0[8:2]} : {wMIPI_DH[0],wMIPI_DL[0],rMIPI_D0[9:2]};
  rMIPI_D1<= rPHASE ? {wMIPI_DH[1],wMIPI_DL[1],rMIPI_D1[8:2]} : {wMIPI_DH[1],wMIPI_DL[1],rMIPI_D1[9:2]};
  rPRESCALE<= rPRESCALE+1;
  rMIPI_DATAVALID<=0;
  rMIPI_PIXELVALID<=0;
  rMIPI_PIXELVALID2<=0;
  if (rMIPI_D0[1:0]==0 && rMIPI_D1[1:0]==0) begin
    if (!rMIPI_ZC[2]) 
      rMIPI_ZC<=rMIPI_ZC+1;
  end else begin
    rMIPI_ZC <=0;
  end
  if ((rMIPI_D0[7:0]==8'b10111000) && (rMIPI_D1[7:0]==8'b10111000) && !rMIPI_VDATA && rMIPI_ZC[2]) begin
    rPRESCALE <=1;
    rMIPI_HDR <= 1;
  end else if ((rMIPI_D0[8:1]==8'b10111000) && (rMIPI_D1[8:1]==8'b10111000) && !rMIPI_VDATA && rMIPI_ZC[2]) begin
    rPRESCALE <=1;
    rMIPI_HDR <= 1;
    rPHASE <=!rPHASE;
    if (!rPHASE) begin
      rMIPI_D0<= {wMIPI_DH[0],wMIPI_DL[0],rMIPI_D0[9:3]};
      rMIPI_D1<= {wMIPI_DH[1],wMIPI_DL[1],rMIPI_D1[9:3]};
    end else begin
      rMIPI_D0<= {wMIPI_DH[0],wMIPI_DL[0],rMIPI_D0[8:1]};
      rMIPI_D1<= {wMIPI_DH[1],wMIPI_DL[1],rMIPI_D1[8:1]};
    end
  end
  else if (rPRESCALE[1:0]==3) begin
    rMIPI_DATAVALID <=1;
  end
  if (rMIPI_DATAVALID)
  begin
    if (rMIPI_HDR)
    begin
      rMIPI_HDR  <= 0;
      rMIPI_HDR2 <= 1;
      case (wMIPI_DATA[7:0])
        cMIPI_HT_VSTART: begin
          rMIPI_VSTART   <= 1; 
          rMIPI_PIXELVALID <=1;
          rMIPI_BAYERPOL <= 0;
        end
        cMIPI_HT_RAW8: begin
          rMIPI_VDATA  <= 1; 
          rWC[7:0]     <= wMIPI_DATA[15:8];
        end
      endcase
    end
    else if (rMIPI_HDR2) begin
      rMIPI_HDR2 <=0;
      rMIPI_VSTART <=0;
      if (rMIPI_VDATA) rWC[15:0] <= {wMIPI_DATA[7:0],rWC[7:0]}-2;
    end
    else if (rMIPI_VDATA) begin
      if (rMIPI_BAYERPOL) begin
        rMIPI_R  <= wMIPI_DATA[15:8];
        rMIPI_G0 <= wMIPI_DATA[7:0];
        rMIPI_B  <= wMIPI_LB_DATA[7:0];
        rMIPI_G1 <= wMIPI_LB_DATA[15:8];
      end else begin
        rMIPI_R  <= wMIPI_LB_DATA[15:8];
        rMIPI_G0 <= wMIPI_LB_DATA[7:0]; 
        rMIPI_B  <= wMIPI_DATA[7:0];
        rMIPI_G1 <= wMIPI_DATA[15:8];     
      end
      rMIPI_PIXELVALID <=1;
      rWC<=rWC-2;
      if (rWC==0) begin
        rMIPI_VDATA<=0;
        if (rMIPI_VDATA)
          rMIPI_BAYERPOL<=!rMIPI_BAYERPOL;
      end else
        rWC<=rWC-2;
    end
  end
  if (rMIPI_PIXELVALID&&!rMIPI_VSTART)
  begin
    rMIPI_PIXELVALID2 <=1;
  end
end

wire [15:0] wMEM_DATA;
wire        wMEM_EMPTY;
reg         rFIFO_RD;

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

    .wrclk        (iMIPI_CLK),   
    .wrreq        (rMIPI_PIXELVALID|rMIPI_PIXELVALID2),              
    .wrempty      (),     
    .wrfull       (),               
    .wrusedw      (),
    .data         ({rMIPI_VSTART,rMIPI_PIXELVALID ? {rMIPI_R[4:0], rMIPI_G0[4:0], rMIPI_B[4:0]} : {rMIPI_R[4:0], rMIPI_G1[4:0], rMIPI_B[4:0]} }),   
 
    .rdclk        (iMEM_CLK),     
    .rdreq        (rFIFO_RD),
    .rdempty      (wMEM_EMPTY),
    .rdusedw      (),
    .rdfull       (),
    .q            (wMEM_DATA) 
  );

reg [31:0] rMEM_ADDRESS;
reg        rMEM_WRITE;
reg [14:0] rMEM_DATA;

  always @(posedge iMEM_CLK)
  begin
    rFIFO_RD<=0;
    if (!wMEM_EMPTY&&!rMEM_WRITE)
    begin
      rMEM_WRITE <=1;
      rFIFO_RD <=1;
      rMEM_DATA <= wMEM_DATA;
      if (wMEM_DATA[15]) rMEM_ADDRESS<=0;
    end
    if (rMEM_WRITE&&!iWAITREQUEST)
    begin
      rMEM_WRITE <=0;
      rMEM_ADDRESS<=rMEM_ADDRESS+1;
    end
  end

  assign oADDRESS=rMEM_ADDRESS;
  assign oDATA=rMEM_DATA;
  assign oWRITE=rMEM_WRITE;
  
endmodule

  