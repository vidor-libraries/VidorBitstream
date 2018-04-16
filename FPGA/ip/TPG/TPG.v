module TPG #(
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
  input iCLK,
  output [7:0] oRED,
  output [7:0] oGRN,
  output [7:0] oBLU,
  output reg   oHS,
  output reg   oVS,
  output reg   oDE
  );

reg [10:0] rHCNT, rVCNT;

assign oRED=rHCNT&{8{oDE}};
assign oGRN=rVCNT&{8{oDE}};
assign oBLU=~rHCNT&{8{oDE}};

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
    if (rVCNT==pVSS-1)
      oVS <=1;
    if (rVCNT==pVSE-1)
      oVS <=0;
  end
  if (rHCNT==pHSE-1)
    oHS <=0;
  if (rHCNT==pHRES-1)
    oDE <=0;
end

endmodule