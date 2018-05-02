module PIO #(
  parameter pBITS =32,
  parameter pMUX_BITS = 2
) 
(
  input iCLK,
  input iRESET,
  
  input [3:0] iADDRESS,
  input iWRITE,
  input iREAD,
  input [31:0] iWRITE_DATA,
  output reg [31:0] oREAD_DATA,
  
  input [pBITS-1:0] iPIO,
  output reg [pBITS-1:0] oPIO,
  output reg [pBITS-1:0] oDIR,
  output reg [pBITS*pMUX_BITS-1:0][1:0] oMUXSEL
);

always @(posedge iCLK)
begin
  if (iWRITE) begin
    case (iADDRESS)
      0: oPIO <= iWRITE_DATA;
      1: oDIR <= iWRITE_DATA;
      2: oPIO <= oPIO&~iWRITE_DATA;
      3: oPIO <= oPIO|iWRITE_DATA;
      4: oMUXSEL[31:0] <= iWRITE_DATA;
      5: oMUXSEL[63:32] <= iWRITE_DATA;
    endcase
  end
  if (iREAD) begin
    case (iADDRESS)
      0: oREAD_DATA <= iPIO;
      1: oREAD_DATA <= oDIR;
      2: oREAD_DATA <= oPIO;
      3: oREAD_DATA <= oPIO;
      4: oREAD_DATA <= oMUXSEL[31:0];
      5: oREAD_DATA <= oMUXSEL[63:32];
    endcase
  end
end

endmodule