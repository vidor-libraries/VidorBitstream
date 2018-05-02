module FBST #(
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
  output            oDE,

  input             iFB_START,
  input [30:0]      iFB_DATA,
  input             iFB_DATAVALID,
  output            oFB_READY
 );

reg [10:0] rHCNT, rVCNT;
reg rFLUSH;
reg rDE;
reg rVBLANK;
assign oFB_READY = (rDE&!rFLUSH&!iFB_START)|(rFLUSH&!iFB_START)| (rVBLANK&&iFB_START);
assign oDE = rDE;

initial begin
  rHCNT <=pHTOTAL-140;
  rVCNT <=pVTOTAL-1;
  rFLUSH <=0;
  rDE <=0;
  rVBLANK <=1;
  oHS<=0;
  oVS<=0;
end
always @(posedge iCLK)
begin
  rHCNT <= rHCNT+1;
  if  (rVBLANK&&iFB_START)
    rFLUSH <=0;
  if (rHCNT==pHTOTAL-1)
  begin
    rHCNT <=0;
    if (rVCNT<pVRES-1)
      rDE<=1;
    else 
      rVBLANK <=1;
    rVCNT <= rVCNT+1;
    if (rVCNT==pVTOTAL-1)
    begin
      rVCNT <=0;
      rDE <=1;
      rVBLANK <=0;
    end
  end
  
  if (rHCNT==pHSS-1)
  begin
    oHS <=1;
    if (rVCNT==pVSS-1) begin
      oVS <=1;
    end
    if (rVCNT==pVSE-1) begin
      oVS <=0;
    end
  end
  if (rHCNT==pHSE-1)
    oHS <=0;
  if (rHCNT==pHRES-1)
    rDE <=0;
  if (oDE && !iFB_DATAVALID || oDE && iFB_START)
    rFLUSH <=1'b1;

end


assign {oBLU[7:3],oGRN[7:3],oRED[7:3]} = iFB_DATA[30] ? iFB_DATA[29:15] : iFB_DATA[14:0];


endmodule