module JTAG_BRIDGE (
  input         iCLK,
  input         iRESET,
  output [31:0] oADDRESS,
  output        oWRITE,
  output        oREAD,
  output [31:0] oWRITE_DATA,
  input  [31:0] iREAD_DATA,
  output [4:0]  oBURST_COUNT,
  input         iWAIT_REQUEST,
  input         iREAD_DATAVALID
);

reg [31:0] rBUFFER, rADDRESS, rDATA;
reg        rCDR,rSDR,rUIR,rUDR,rADDRESS_PHASE,rADDRESS_PHASED/* synthesis noprune */;
reg [1:0]  rIR;
reg [4:0]  rBITCNT;
reg        rWRITE_PHASE,rWR_STROBE, rRD_STROBE;
wire       wWRITE_PHASE, wRD_EMPTY,wWR_EMPTY;
wire [31:0] wREAD_DATA;
wire [1:0] wIR_OUT, wIR_IN;
wire wTDI,wTDO,wTCK;
wire wCIR,wPDR,wUIR,wSDR,wCDR,wUDR,wE1DR,wE2DR;
reg [4:0] rDATA_CNT;
reg [1:0] rADDR_BITS;

    sld_virtual_jtag_basic  sld_virtual_jtag_component (
                .ir_out (wIR_OUT),
                .tdo (wTDO),
                .tdi (wTDI),
                .tck (wTCK),
                .ir_in (wIR_IN),
                .virtual_state_cir  (wCIR),
                .virtual_state_pdr  (wPDR),
                .virtual_state_uir  (wUIR),
                .virtual_state_sdr  (wSDR),
                .virtual_state_cdr  (wCDR),
                .virtual_state_udr  (wUDR),
                .virtual_state_e1dr (wE1DR),
                .virtual_state_e2dr (wE2DR)
                // synopsys translate_off
                ,
                .jtag_state_cdr (),
                .jtag_state_cir (),
                .jtag_state_e1dr (),
                .jtag_state_e1ir (),
                .jtag_state_e2dr (),
                .jtag_state_e2ir (),
                .jtag_state_pdr (),
                .jtag_state_pir (),
                .jtag_state_rti (),
                .jtag_state_sdr (),
                .jtag_state_sdrs (),
                .jtag_state_sir (),
                .jtag_state_sirs (),
                .jtag_state_tlr (),
                .jtag_state_udr (),
                .jtag_state_uir (),
                .tms ()
                // synopsys translate_on
                );
    defparam
        sld_virtual_jtag_component.sld_mfg_id = 110,
        sld_virtual_jtag_component.sld_type_id = 132,
        sld_virtual_jtag_component.sld_version = 1,
        sld_virtual_jtag_component.sld_auto_instance_index = "YES",
        sld_virtual_jtag_component.sld_instance_index = 0,
        sld_virtual_jtag_component.sld_ir_width = 3,
        sld_virtual_jtag_component.sld_sim_action = "",
        sld_virtual_jtag_component.sld_sim_n_scan = 0,
        sld_virtual_jtag_component.sld_sim_total_length = 0;

        reg cdr_delayed,sdr_delayed,uir_delayed ,udr_delayed  ;

assign wTDO=rBUFFER[0];

always @(negedge wTCK)
begin
  //  Delay the CDR signal by one half clock cycle 
  rCDR <= wCDR;
  rSDR <= wSDR;
  rUIR <= wUIR;
  rUDR <= wUDR;
  if( wUIR ) rIR <= wIR_IN;
end


always @(posedge wTCK)
begin
  rWR_STROBE<=0;
  rRD_STROBE<=0;
  if (rCDR)
  begin
    rBUFFER<=0;
    rBITCNT<=0;
  end
  if (rSDR)
  begin
    rBUFFER <= {wTDI,rBUFFER[31:1]};
    rBITCNT<=rBITCNT+1;
  end
  if (rWR_STROBE)
    rADDRESS<=rADDRESS+1;
  case (rIR)
    0: begin
      if (rCDR)
      begin
        rADDRESS_PHASE<=1;
        rWRITE_PHASE<=0;
      end
      if (rUDR)
      begin
        rADDRESS_PHASE<=0;
        rWRITE_PHASE<=0;
        if (rBITCNT==3)
        begin
          rDATA_CNT<={1'b0,wTDI,rBUFFER[31-:3]}+1;
        end
        else rDATA_CNT<=1;
      end
      if (rSDR) begin
        // address phase. depending on header we shift in only part of the address
        if (rADDRESS_PHASE) begin
          case (rBITCNT)
          1: rADDR_BITS <= {wTDI,rBUFFER[31]};
          7: if (rADDR_BITS==0) begin
               rADDRESS[5:0]<={wTDI,rBUFFER[31-:5]};
               rBITCNT<=0;
               rADDRESS_PHASE<=0;
               rWRITE_PHASE<=1;
             end
          15:if (rADDR_BITS==1) begin
               rADDRESS[13:0]<={wTDI,rBUFFER[31-:13]};
               rBITCNT<=0;
               rADDRESS_PHASE<=0;
               rWRITE_PHASE<=1;
             end
          23:if (rADDR_BITS==2) begin
               rADDRESS[21:0]<={wTDI,rBUFFER[31-:21]};
               rBITCNT<=0;
               rADDRESS_PHASE<=0;
               rWRITE_PHASE<=1;
             end
          31:if (rADDR_BITS==3) begin
               rADDRESS[29:0]<={wTDI,rBUFFER[31-:29]};
               rBITCNT<=0;
               rADDRESS_PHASE<=0;
               rWRITE_PHASE<=1;
             end
          endcase
        end
        else begin
          if (rBITCNT==31) begin
            rDATA<={wTDI,rBUFFER[31-:31]};
            rWR_STROBE<=1;
          end
        end
      end
    end
    1: begin
      if (rCDR) begin
        rBITCNT<=0;
        rBUFFER<=wREAD_DATA;
        rRD_STROBE<=1;
      end
      if (rSDR) begin
        if (rBITCNT==31) begin
          rBUFFER<=wREAD_DATA;
          rRD_STROBE<=1;
        end
      end
    end
  endcase
end

/*
always @(posedge wTCK)
begin
  rWR_STROBE<=0;
  rRD_STROBE<=0;
  rADDRESS_PHASED<=rADDRESS_PHASE;
  if (rUIR && rIR==0)
    rADDRESS_PHASE <=1;
  if (rUDR) begin
    rADDRESS_PHASE <=0; 
    if (rADDRESS_PHASED) begin 
      rWR_STROBE <=1; // perform read request
    end
    rBITCNT<=0;
    rWRITE_PHASE<=0;
  end
  if (rWR_STROBE)
    rADDRESS<=rADDRESS+1;
  case (rIR)
    0: begin
      if (rADDRESS_PHASE) begin
         if( rCDR ) begin
           rBITCNT<=0;
           rADDRESS<=0;
         end
         else if( rSDR ) begin
           rADDRESS <= {wTDI,rADDRESS[31:1]};
           rBITCNT<=rBITCNT+1;
           if (rBITCNT==31) begin
              rADDRESS_PHASE<=0; 
              rWRITE_PHASE<=1;
           end
        end     
      end
      else begin
        if (rCDR&!rWRITE_PHASE) begin
          rDATA<=wREAD_DATA;
          rRD_STROBE<=1;
          rBITCNT<=0;
        end
        if( rSDR )   begin
          rDATA <= {wTDI,rDATA[31:1]};
          rBITCNT<=rBITCNT+1;
        end
      end
    end
  endcase;
end
*/

  dcfifo  #(
    .add_usedw_msb_bit("ON"),
    .intended_device_family("Cyclone 10 LP"),
    .lpm_numwords(4),
    .lpm_showahead("ON"),
    .lpm_type("dcfifo"),
    .lpm_width(70),
    .lpm_widthu(2),
    .overflow_checking("OFF"),
    .rdsync_delaypipe(5),
    .read_aclr_synch("ON"),
    .underflow_checking("ON"),
    .use_eab("OFF"),
    .write_aclr_synch("OFF"),
    .wrsync_delaypipe(5)
  ) fifowr_inst (
    .data       ({rADDRESS,rDATA,rDATA_CNT,rWRITE_PHASE}),
    .rdclk      (iCLK),
    .rdreq      (!wWR_EMPTY&&!iWAIT_REQUEST),
    .wrclk      (wTCK),
    .wrreq      (rWR_STROBE|(rUIR&&rIR==1)),
    .q          ({oADDRESS,oWRITE_DATA,oBURST_COUNT,wWRITE_PHASE}),
    .rdempty    (wWR_EMPTY),
    .aclr       (),
    .rdfull     (),
    .rdusedw    (),
    .eccstatus  (),
    .wrempty    (),
    .wrfull     (),
    .wrusedw    ());


assign oWRITE = !wWR_EMPTY& wWRITE_PHASE;
assign oREAD  = !wWR_EMPTY&!wWRITE_PHASE;

  dcfifo  #(
    .add_usedw_msb_bit("ON"),
    .intended_device_family("Cyclone 10 LP"),
    .lpm_numwords(4),
    .lpm_showahead("ON"),
    .lpm_type("dcfifo"),
    .lpm_width(32),
    .lpm_widthu(2),
    .overflow_checking("OFF"),
    .rdsync_delaypipe(5),
    .read_aclr_synch("ON"),
    .underflow_checking("ON"),
    .use_eab("OFF"),
    .write_aclr_synch("OFF"),
    .wrsync_delaypipe(5)
  ) fiford_inst (
    .data       (iREAD_DATA),
    .rdclk      (wTCK),
    .rdreq      (rRD_STROBE&&!wRD_EMPTY),
    .wrclk      (iCLK),
    .wrreq      (iREAD_DATAVALID),
    .q          (wREAD_DATA),
    .rdempty    (wRD_EMPTY),
    .aclr       (),
    .rdfull     (),
    .rdusedw    (),
    .eccstatus  (),
    .wrempty    (),
    .wrfull     (),
    .wrusedw    ());


endmodule