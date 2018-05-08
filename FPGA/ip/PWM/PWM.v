

module PWM #(
  parameter pCHANNELS=16,
  parameter pPRESCALER_BITS=32,
  parameter pMATCH_BITS=32

) 
(
  iCLK,
  iRESET,
  
  iADDRESS,
  iDATA,
  iWRITE,
  
  oPWM
);

  //----------------------------------------------------------------------------------------------------------
          //  ceil of the log base 2
  //----------------------------------------------------------------------------------------------------------
  function integer CLogB2;
    input [31:0] Depth;
    integer i;
    begin
      i = Depth-1;        
      for(CLogB2 = 0; i > 0; CLogB2 = CLogB2 + 1)
        i = i >> 1;
    end
  endfunction

  input iCLK;
  input iRESET;
  
  input [CLogB2(pCHANNELS+1):0]  iADDRESS;
  input [31:0] iDATA;
  input        iWRITE;
  
  output reg [pCHANNELS-1:0] oPWM;

  
reg [pPRESCALER_BITS-1:0] rPRESCALER_CNT;
reg [pPRESCALER_BITS-1:0] rPRESCALER_MAX;

reg [pMATCH_BITS-1:0] rPERIOD_CNT;
reg [pMATCH_BITS-1:0] rPERIOD_MAX;
reg [pMATCH_BITS-1:0] rMATCH_H [pCHANNELS-1:0];
reg [pMATCH_BITS-1:0] rMATCH_L [pCHANNELS-1:0];
reg rTICK;
integer i;

always @(posedge iCLK)
begin
  if (iWRITE) begin
    if (iADDRESS>=2) case (iADDRESS[0])
      0: rMATCH_H[iADDRESS[CLogB2(pCHANNELS):1]-1]<= iDATA;
      1: rMATCH_L[iADDRESS[CLogB2(pCHANNELS):1]-1]<= iDATA;
    endcase
    else begin
	   case (iADDRESS[0])
		  0: rPRESCALER_MAX<=iDATA;
		  1: rPERIOD_MAX<=iDATA;
		endcase
    end
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
  for (i=0;i<pCHANNELS;i=i+1) begin
    if (rMATCH_H[i]==rPERIOD_CNT)
      oPWM[i] <=1;
    if (rMATCH_L[i]==rPERIOD_CNT)
      oPWM[i] <=0;
  end
end

endmodule