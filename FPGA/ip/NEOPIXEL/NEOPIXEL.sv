module NEOPIXEL #(
  pCHANNELS=23,
  pSTART_ADDRESS=0
  ) (
  input             iCLOCK,
  input             iRESET,

  input  [3:0]	     iCSR_ADDRESS,
  input        	  iCSR_READ,
  output reg [31:0] oCSR_READ_DATA,
  input        	  iCSR_WRITE,
  input  [31:0]	  iCSR_WRITE_DATA,

  output reg [31:0] oDATA_ADDRESS,
  output reg        oDATA_READ,
  input             iDATA_WAITREQUEST,
  output [4:0]      oDATA_BURST_COUNT,
  input  [31:0]     iDATA_READ_DATA,
  input             iDATA_READ_DATAVALID,

  output reg                     oIRQ,
  output reg [pCHANNELS-1:0]      oDATA
);

localparam pCHANNEL_BITS = $clog2(pCHANNELS);

typedef enum reg [1:0] {stIDLE, stPRELOAD,stSHIFTING,stRESETTING} eSTATE;

eSTATE rINT_STATE;
reg [4:0] rBIT_COUNTER, rBIT_COUNT;
reg [15:0] rCOUNTER;
reg [15:0] rT0H, rT1H, rTT, rTRESET;
reg [9:0]  rSTRING_LEN;
reg [pCHANNELS-1:0][23:0] rPIXEL_DATA;
reg [pCHANNELS-1:0][23:0] rTMP_DATA;
reg [pCHANNELS-1:0] rMASK;
reg [pCHANNEL_BITS-1:0] rCHAN_CNT;
reg rSTART;
integer i;
assign oDATA_BURST_COUNT = pCHANNELS;


always @(posedge iCLOCK)
begin
  if (iCSR_WRITE) begin
    case (iCSR_ADDRESS)
      0: rMASK <= iCSR_WRITE_DATA;
      1: begin
        {rSTART,rSTRING_LEN, rBIT_COUNT} <= iCSR_WRITE_DATA;
        oIRQ<=0;
      end
      2: rTRESET <= iCSR_WRITE_DATA;
      3: rT0H <= iCSR_WRITE_DATA;
      4: rT1H <= iCSR_WRITE_DATA;
      5: rTT <= iCSR_WRITE_DATA;
    endcase
  end
  if (iCSR_READ) begin
    case (iCSR_ADDRESS)
      0: oCSR_READ_DATA <= rMASK;
      1: oCSR_READ_DATA <= {rSTART, rSTRING_LEN, rBIT_COUNT};
      7: oCSR_READ_DATA <= rINT_STATE;
    endcase
  end
  if ((rINT_STATE==stIDLE) && rSTART) begin
    rINT_STATE<= stPRELOAD;
    rCHAN_CNT<= 0;
    oDATA_READ <= 1;
    oDATA_ADDRESS <= pSTART_ADDRESS;
    rSTART<= 0;
  end
  if (oDATA_READ&&!iDATA_WAITREQUEST) begin
    oDATA_READ<= 0;
    oDATA_ADDRESS <= oDATA_ADDRESS+{oDATA_BURST_COUNT,2'b0};
  end
  
  if (iDATA_READ_DATAVALID) begin
    rCHAN_CNT<= rCHAN_CNT+1;
    rTMP_DATA[rCHAN_CNT]<= iDATA_READ_DATA;
    if (rCHAN_CNT==pCHANNELS-1) begin
      rCHAN_CNT <= 0;
      if (rINT_STATE==stPRELOAD) begin
        rPIXEL_DATA<= rTMP_DATA;
        rINT_STATE<= stSHIFTING;
        rCOUNTER <= 0;
        rBIT_COUNTER<= 0;
        oDATA_READ <= 1;
      end
    end
  end

  if (rINT_STATE==stSHIFTING) begin
    rCOUNTER<= rCOUNTER+1;
    if (rCOUNTER==0) begin
      oDATA<= rMASK;
    end
    if (rCOUNTER==rT1H) begin
      for (i=0;i<pCHANNELS;i++) begin
        if (rPIXEL_DATA[i][rBIT_COUNT]) oDATA[i]<=0;
      end
    end
    if (rCOUNTER==rT0H) begin
      for (i=0;i<pCHANNELS;i++) begin
        if (!rPIXEL_DATA[i][rBIT_COUNT]) oDATA[i]<=0;
      end
    end
    if (rCOUNTER==rTT) begin
      rCOUNTER<=0;
      rBIT_COUNTER<=rBIT_COUNTER+1;
      if (rBIT_COUNTER==rBIT_COUNT) begin
        rBIT_COUNTER<= 0;

        if (rSTRING_LEN==0) begin
          rINT_STATE<= stRESETTING;
          oDATA <=0;
        end
        else begin
          rSTRING_LEN<= rSTRING_LEN-1;
          oDATA_READ <= 1;
          rPIXEL_DATA<= rTMP_DATA;
        end
      end
      else begin
        for (i=0;i<pCHANNELS;i++) begin
          rPIXEL_DATA[i] <= rPIXEL_DATA[i]<<1;
        end
      end
    end
  end
  if (rINT_STATE==stRESETTING) begin
    rCOUNTER <= rCOUNTER+1;
    if (rCOUNTER==rTRESET) begin
      rCOUNTER <= 0;
      rINT_STATE <= stIDLE;
      oIRQ<= 1;
    end
  end
end


endmodule

