module FB #(
  parameter pHRES=1280,
  parameter pVRES=720,
  parameter pHTOTAL=1354,
  parameter pVTOTAL=910,
  parameter pHSS=1300,
  parameter pHSE=1340,
  parameter pVSS=778,
  parameter pVSE=782
 )
 
 (
  input             iCLK,
  output [7:0]      oRED,
  output [7:0]      oGRN,
  output [7:0]      oBLU,
  output reg        oHS,
  output reg        oVS,
  output reg        oDE,

  input             iMEM_CLK,
  input             iMEM_RESET,
  input      [15:0] iMEM_DATA,
  output reg [19:0] oMEM_ADDR,
  output reg        oMEM_READ,
  input             iMEM_DATAVALID,
  input             iMEM_WAIT
  );

reg [10:0] rHCNT, rVCNT;
reg rFLUSH;

always @(posedge iCLK)
begin
  rHCNT <= rHCNT+1;
  if (rHCNT==pHTOTAL-1)
  begin
    rHCNT <=0;
    if (rVCNT<pVRES-1)
      oDE<=1;
    rVCNT <= rVCNT+1;
    if (rVCNT==pVTOTAL-1)
    begin
      rVCNT <=0;
      oDE <=1;
    end
  end
  
  if (rHCNT==pHSS-1)
  begin
    oHS <=1;
    if (rVCNT==pVSS-1) begin
      oVS <=1;
      rFLUSH<=1;
    end
    if (rVCNT==pVSE-1) begin
      oVS <=0;
      rFLUSH<=0;
    end
  end
  if (rHCNT==pHSE-1)
    oHS <=0;
  if (rHCNT==pHRES-1)
    oDE <=0;
  if (oDE && wFR_RD_EMPTY)
    rFLUSH <=1'b1;
end

// frame reader
wire [7:0]  wFR_FIFO_UDW;

reg [21:0] wFR_NEXT_RDADDR;

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

    .wrclk        (iMEM_CLK),   
    .wrreq        (iMEM_DATAVALID),              
    .wrempty      (),     
    .wrfull       (),               
    .wrusedw      (wFR_FIFO_UDW),
    .data         (iMEM_DATA),   
 
    .rdclk        (iCLK),     
    .rdreq        ((oDE|rFLUSH)&&!wFR_RD_EMPTY),
    .rdempty      (wFR_RD_EMPTY),                                      
    .rdusedw      (),
    .rdfull       (),
    .q            ({oBLU[7:3],oGRN[7:3],oRED[7:3]}) 
  );

reg [2:0] rFLUSH_RESYNC;


always @(posedge iMEM_CLK)
begin
  rFLUSH_RESYNC <= {rFLUSH_RESYNC,rFLUSH};
  if (oMEM_READ&&!iMEM_WAIT)
  begin
    oMEM_READ <=0;
    oMEM_ADDR<= oMEM_ADDR+1;
  end
  
  if ((wFR_FIFO_UDW<64)&&!rFLUSH_RESYNC[2]&&!(oMEM_ADDR==(pHRES*pVRES-1)))
  begin
    oMEM_READ <=1;
  end
  if (rFLUSH_RESYNC[2])
    oMEM_ADDR<=0;
end

endmodule