module QUAD_ENCODER #(
    pENCODERS=2
  )(
    input                             iCLK,
    input                             iRESET,
    // AVALON SLAVE INTERFACE
    input  [$clog2(pENCODERS)-1:0]	  iAVL_ADDRESS,
    input        	                    iAVL_READ,
    output reg [31:0]                 oAVL_READ_DATA,
    // ENCODER INPUTS
    input [pENCODERS-1:0]             iENCODER_A,
    input [pENCODERS-1:0]             iENCODER_B
  );

  

reg [3:0][pENCODERS-1:0] rRESYNC_ENCODER_A,rRESYNC_ENCODER_B;
reg [pENCODERS-1:0][15:0] rSTEPS;

// decrement
// A __----____----__
// B ____----____----
//     
// increment
// A ____----____----
// B __----____----__

wire [pENCODERS-1:0] wENABLE =  rRESYNC_ENCODER_A[2]^rRESYNC_ENCODER_A[3]^rRESYNC_ENCODER_B[2]^rRESYNC_ENCODER_B[3];
wire [pENCODERS-1:0] wDIRECTION = rRESYNC_ENCODER_A[2]^rRESYNC_ENCODER_B[3];
integer i;

initial rSTEPS <=0;

always @(posedge iCLK)
begin
  if (iRESET) begin
    rSTEPS<=0;
    rRESYNC_ENCODER_A<=0;
    rRESYNC_ENCODER_B<=0;
  end
  else begin
    rRESYNC_ENCODER_A<={rRESYNC_ENCODER_A,iENCODER_A};
    rRESYNC_ENCODER_B<={rRESYNC_ENCODER_B,iENCODER_B};
    for (i=0;i<pENCODERS;i=i+1)
    begin
      if (wENABLE[i])
        rSTEPS[i] <= rSTEPS[i]+ ((wDIRECTION[i]) ? 1 : -1);
    end

    if (iAVL_READ)
    begin
        oAVL_READ_DATA<= rSTEPS[iAVL_ADDRESS];
    end
  end
end

endmodule
