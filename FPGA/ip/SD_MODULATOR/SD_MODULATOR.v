module SD_MODULATOR #(
  parameter pBITS=24,
  parameter pINTERP_BITS =5
) 
(
  input             iCLK,
  input [pBITS-1:0] iDATA,
  output reg        oSTROBE,
  output            oDAC
);

reg [pBITS-1:0]              rDATA,
                             rDDATA;
reg [pBITS+pINTERP_BITS-1:0] rINT_DATA;
reg [pINTERP_BITS-1:0]       rCNT;
/*reg [pBITS+1:0]              rDELTA_ADD,
                             rSIGMA_ADD,
                             rSIGMA_D;
assign   oDAC = rSIGMA_D[pBITS+1];
*/
reg [pBITS:0] rACC;
assign   oDAC = rACC[pBITS];

always @(posedge iCLK)
begin

  rCNT<=rCNT+1;
  oSTROBE<=0;
  // perform linear interpolation between samples
  if (rCNT==0) begin
    oSTROBE<=1;
    rDATA  <= iDATA;
    rDDATA <= rDATA;
    rINT_DATA<={rDATA,{pINTERP_BITS{1'b0}} };
  end else begin
    rINT_DATA<=rINT_DATA+{{pINTERP_BITS{1'b0}},rDATA}-{{pINTERP_BITS{1'b0}},rDDATA};
  end

  rACC <= {1'b0,rACC[pBITS-1:0]}+rINT_DATA[pBITS+pINTERP_BITS-1:pINTERP_BITS];
  //rDELTA_ADD <= rINT_DATA[pBITS+pINTERP_BITS-1 -:pBITS] + {rSIGMA_D[pBITS],rSIGMA_D[pBITS],{pBITS-1{1'b0}}};
  //rSIGMA_ADD <= rDELTA_ADD+rSIGMA_D;
  //rSIGMA_D <= rSIGMA_ADD;

end

endmodule
