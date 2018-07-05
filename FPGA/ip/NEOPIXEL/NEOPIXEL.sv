module NEOPIXEL #(
  pCHANNELS=23
  ) (
  input         iCLOCK,
  input         iRESET,

  input  [3:0]	iCSR_ADDRESS,
  input        	iCSR_READ,
  output reg [31:0]	oCSR_READ_DATA,
  input        	iCSR_WRITE,
  input  [31:0]	iCSR_WRITE_DATA,

  input  [$clog2(pCHANNELS)-1:0]	iDATA_ADDRESS,
  input        	iDATA_WRITE,
  input  [31:0]	iDATA_WRITE_DATA,

  output reg                     oIRQ,
  output reg [pCHANNELS-1:0]      oDATA
);

typedef enum reg [1:0] {stIDLE, stSHIFTING,stRESETTING} eSTATE;

eSTATE rINT_STATE;
reg [4:0] rBIT_COUNTER, rBIT_COUNT;
reg [15:0] rCOUNTER;
reg [15:0] rT0H, rT1H, rTT, rTRESET;

reg [pCHANNELS-1:0][23:0] rPIXEL_DATA;
reg [pCHANNELS-1:0][23:0] rTMP_DATA;
reg [pCHANNELS-1:0] rMASK;
reg rSTART;
reg rSTOP;
integer i;

always @(posedge iCLOCK)
begin
  if (iCSR_WRITE) begin
    case (iCSR_ADDRESS)
      0: rMASK <= iCSR_WRITE_DATA;
      1: begin
        {rSTOP,rSTART, rBIT_COUNT} <= iCSR_WRITE_DATA;
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
      1: oCSR_READ_DATA <= {rSTOP,rSTART, rBIT_COUNT};
      7: oCSR_READ_DATA <= rINT_STATE;
    endcase
  end
  if (iDATA_WRITE)
    rTMP_DATA[iDATA_ADDRESS]<= iDATA_WRITE_DATA;
  if ((rINT_STATE==stIDLE) && rSTART) begin
    rPIXEL_DATA<= rTMP_DATA;
    rINT_STATE<= stSHIFTING;
    rCOUNTER <= 0;
    rBIT_COUNTER<= 0;
    rSTART<= 0;
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
        oIRQ<=1;
        if (rSTOP) begin
          rSTOP <= 0;
          rINT_STATE<= stRESETTING;
          oDATA <=0;
        end
        else begin
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
      if (rSTART) begin
        rINT_STATE <= stSHIFTING;
        rSTART <= 0;
        rPIXEL_DATA  <= rTMP_DATA;
        rBIT_COUNTER <= 0;
      end else rINT_STATE <= stIDLE;
    end
  end
end

endmodule

