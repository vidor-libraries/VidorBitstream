module mailbox_tb();

reg rCLK;
initial rCLK <= 1;
reg [3:0] rRESET;
initial rRESET <= 4'b1111;
always
#1 rCLK <= !rCLK;

reg [2*(32+10+1+1)+1-1 : 0] rSEQ [18+19+16:0] =
'{
//                 MASTER                           SLAVE
// REQ        DATA      ADDR   RD   WR          DATA      ADDR   RD   WR
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h000,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read status
  {1'b0,32'h00000000,10'h3ff,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read response
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000012,10'h3F0,1'b0,1'b1 }, // write event (dropped due to overflow)
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000011,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000010,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h0000000F,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h0000000E,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h0000000D,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h0000000C,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h0000000B,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h0000000A,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000009,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000008,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000007,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000006,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000005,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000004,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000003,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h3F2,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read extra data (should not increment read)
  {1'b0,32'h00000000,10'h3F2,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read data
  {1'b0,32'h00000000,10'h3F2,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read data
  {1'b0,32'h00000000,10'h3F1,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read msg len
  {1'b0,32'h00000000,10'h000,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read status
  {1'b0,32'h00000000,10'h3ff,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read response
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000002,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000001,10'h3F0,1'b0,1'b1 }, // write event
  {1'b0,32'h00000000,10'h000,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read response
  {1'b0,32'h00000000,10'h3ff,1'b1,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // read status
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h000BAC10,10'h000,1'b0,1'b1 }, // ack instruction
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000000,10'h001,1'b1,1'b0 }, // read instruction parameter
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000000,10'h000,1'b1,1'b0 }, // read instruction
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // latency
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // latency 
  {1'b0,32'h00000000,10'h000,1'b0,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // latency
  {1'b1,32'h00000000,10'h000,1'b0,1'b0,  32'h00000000,10'h000,1'b0,1'b0 }, // issue interrupt
  {1'b0,32'hBAC10000,10'h001,1'b0,1'b1,  32'h00000000,10'h000,1'b0,1'b0 },
  {1'b0,32'h0000ADD0,10'h000,1'b0,1'b1,  32'h00000000,10'h000,1'b0,1'b0 } // write instruction
};

reg [7:0] rCNT;
initial rCNT <= 0;
always @(posedge rCLK)
begin
  rRESET <= {rRESET,1'b0};
  if (!rRESET[3])
    rCNT <= rCNT+1;
end

MAILBOX mbox(
  .iCLOCK(rCLK),
  .iRESET(rRESET[3]),
  
  .iMST_ADDRESS         (rSEQ[rCNT][55:46]),
  .iMST_WRITE           (rSEQ[rCNT][44]),
  .iMST_READ            (rSEQ[rCNT][45]),
  .iMST_WRITE_DATA      (rSEQ[rCNT][87:56]),
  .oMST_READ_DATA       (),

  .iSLV_ADDRESS         (rSEQ[rCNT][11:2]),
  .iSLV_WRITE           (rSEQ[rCNT][0]),
  .iSLV_READ            (rSEQ[rCNT][1]),
  .iSLV_WRITE_DATA      (rSEQ[rCNT][43:12]),
  .oSLV_READ_DATA       (),

  .iMST_RQ              (rSEQ[rCNT][88]),
  .oMST_AK              ()
  );
endmodule
