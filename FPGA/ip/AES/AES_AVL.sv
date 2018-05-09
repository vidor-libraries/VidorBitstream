module AES_AVL
(
  input iCLK,
  input iRESET,
  input [3:0]  iADDRESS,
  input [31:0] iWRITEDATA,
  input        iWRITE,
  input        iREAD,
  output [31:0] oREADDATA,
  output        oWAITREQUEST
);

reg rKEY_START,rENDE;
reg [1:0] rKEY_MODE;
reg [7:0][31:0] rKEY;
reg [3:0][31:0] rDATA;
reg rDATAVALID;
wire wREADY;
reg rOUTVALID;
wire wOUTVALID;
wire [3:0][31:0] wOUTDATA;
reg [3:0][31:0] rOUTDATA;
reg rENABLE;

assign oWAITREQUEST=0;
aes u_aes 
(
   .clk(iCLK),
   .reset(iRESET),
   .i_start(rKEY_START),
   .i_enable(rENABLE),
   .i_ende(rENDE),
   .i_key(rKEY),
   .i_key_mode(rKEY_MODE),
   .i_data(rDATA),
   .i_data_valid(rDATAVALID&wREADY&wKEY_READY),
   .o_ready(wREADY),
   .o_data(wOUTDATA),
   .o_data_valid(wOUTVALID),
   .o_key_ready(wKEY_READY)
);


always @(posedge iCLK)
begin
  rENABLE <= !rENABLE;
  if (rENABLE) begin
    rKEY_START<=0;
    rDATAVALID<=0;
  end
  
  if (wREADY&wKEY_READY) rDATAVALID <=0;
  
  if (iWRITE) begin
    case (iADDRESS)
      0: begin
        {rENDE,rKEY_MODE} <=iWRITEDATA;
        rKEY_START <=1;
      end
    endcase
    if (iADDRESS[3:2]==2'b01) // addresses from 4 to 7
      rDATA[iADDRESS[1:0]]<=iWRITEDATA;
    if (iADDRESS==7)
      rDATAVALID<=1;
    if (iADDRESS[3]==1'b1) // addresses from 8 to 15
      rKEY[iADDRESS[2:0]]<=iWRITEDATA;
  end
  if (wOUTVALID) begin
    rOUTVALID <=1;
    rOUTDATA <= wOUTDATA;
  end
  if (iREAD) begin
    case (iADDRESS)
      0: begin
        oREADDATA <={rENDE,rKEY_MODE};
      end
    endcase
    if (iADDRESS[3:2]==2'b01) // addresses from 4 to 7
      oREADDATA<= rOUTVALID ? 32'hDEADBEEF : rOUTDATA[iADDRESS[1:0]];
    if (iADDRESS==7)
      rOUTVALID<=0;
  end
end

endmodule