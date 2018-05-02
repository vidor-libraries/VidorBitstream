module #(
  parameter pCHANNELS=16,
  parameter pPRESCALER_BITS=32,
  parameter pMATCH_BITS=32
  
) PWM
(
  input iCLK,
  input iRESET,
  
  input [3:0]  iADDRESS,
  input [31:0] iDATA,
  input        iWRITE,
  
  output reg [pCHANNELS-1:0] oPWM
);

reg [pPRESCALER_BITS-1:0] rPRESCALER_CNT;
reg [pPRESCALER_BITS-1:0] rPRESCALER_MAX;

reg [pMATCH_BITS-1:0] rPERIOD_CNT;
reg [pMATCH_BITS-1:0] rPERIOD_MAX;
reg [pMATCH_BITS-1:0][pCHANNELS-1:0] rMATCH_H;
reg [pMATCH_BITS-1:0][pCHANNELS-1:0] rMATCH_L;
reg rTICK;
integer i;

always @(posedge iCLK)
begin
  if (iWRITE) begin
    case (iADDRESS[0])
      0: rMATCH_H[iADDRESS[3:2]]<= iDATA;
      1: rMATCH_L[iADDRESS[3:2]]<= iDATA;
    endcase
  end
        
        
  rPRESCALER_CNT<=rPRESCALER_CNT+1;
  rTICK<=0;
  if (rPRESCALER_CNT>= rPRESCALER_MAX) begin
    rPRESCALER_CNT<=0;
    rTICK <=1;
  end
  if (rTICK) begin
    rPERIOD_CNT<=rPERIOD_CNT+1;
    if (rPERIOD_CNT>=rPERIOD_MAX) begin
      rPERIOD_CNT<=0;
    end
  end
  for (i=0;i<pCHANNELS;i++) begin
    if (rMATCH_H==rPERIOD_CNT)
      oPWM[i] <=1;
    if (rMATCH_L==rPERIOD_CNT)
      oPWM[i] <=0;
  end
end

endmodule