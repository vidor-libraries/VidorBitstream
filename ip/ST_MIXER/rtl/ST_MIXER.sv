module ST_MIXER #(
  parameter pBIT_WIDTH=16,
  parameter pALPHA_BITS=1,
  parameter pBG_HRES=640,
  parameter pBG_VRES=480
)
(
  input                   iCLK,
  input                   iRESET,

  input  [3:0]            iAV_ADDRESS,
  input  [31:0]           iAV_WRITE_DATA,
  input                   iAV_WRITE,

  input  [pBIT_WIDTH-1:0] iBG_DATA,
  input                   iFG_ALPHA,
  input                   iBG_VALID,
  output                  oBG_READY,
  input                   iBG_SOP,
  input                   iBG_EOP,

  input  [pBIT_WIDTH-1:0] iFG_DATA,
  input                   iFG_VALID,
  output                  oFG_READY,
  input                   iFG_SOP,
  input                   iFG_EOP,

  output reg [pBIT_WIDTH-1:0] oOUT_DATA,
  output reg                  oOUT_VALID,
  input                       iOUT_READY,
  output reg                  oOUT_SOP,
  output reg                  oOUT_EOP

);

localparam cHCNT_WIDTH = $clog2(pBG_HRES);
localparam cVCNT_WIDTH = $clog2(pBG_VRES);
localparam cBG_EOP_BIT = pBIT_WIDTH;
localparam cBG_SOP_BIT = pBIT_WIDTH+1;
localparam cFG_EOP_BIT = pBIT_WIDTH+pALPHA_BITS;
localparam cFG_SOP_BIT = pBIT_WIDTH+pALPHA_BITS+1;
localparam cFG_ALPHA_BIT= pBIT_WIDTH;

enum reg [1:0] {stSYNCING,stSYNCHED,stVIDEO,stPAUSE} rFG_STATE, rBG_STATE;

reg rFG_ENABLED, rREG_FG_ENABLED, rVPFLAG;
reg [cHCNT_WIDTH-1:0] rHCNT_BG,rREG_BG_HRES,rREG_FG_HRES,rBG_HRES,rHEND,rREG_HPOS, rHPOS;
reg [cVCNT_WIDTH-1:0] rVCNT_BG,rREG_BG_VRES,rREG_FG_VRES,rBG_VRES,rVEND,rREG_VPOS, rVPOS;
reg [3+pBIT_WIDTH-1:0] rFG_DATA;
reg [2+pBIT_WIDTH-1:0] rBG_DATA;
reg                    rBG_EOP;

assign oBG_READY = iOUT_READY&(rBG_STATE==stVIDEO)&(iFG_VALID|!rFG_ENABLED|(rFG_STATE!=stVIDEO))|(rBG_STATE==stSYNCING);
assign oFG_READY = iOUT_READY&(rFG_STATE==stVIDEO)&iBG_VALID|(rFG_STATE==stSYNCING);

always @(posedge iCLK) begin
  if (iRESET) begin
    rFG_STATE       <= stSYNCING;
    rBG_STATE       <= stSYNCING;
    rREG_FG_ENABLED <= 1;
    rREG_BG_HRES    <= 640;
    rREG_BG_VRES    <= 480;
    rREG_HPOS       <= 0;
    rREG_VPOS       <= 0;
    rREG_FG_HRES    <= 160;
    rREG_FG_VRES    <= 160;
    rBG_EOP         <= 0;
end else begin
    
    if (iAV_WRITE) begin
      case (iAV_ADDRESS)
        0: rREG_FG_ENABLED <= iAV_WRITE_DATA[0];
        1: rREG_BG_HRES <= iAV_WRITE_DATA;
        2: rREG_BG_VRES <= iAV_WRITE_DATA;
        3: rREG_HPOS <= iAV_WRITE_DATA;
        4: rREG_VPOS <= iAV_WRITE_DATA;
        5: rREG_FG_HRES <= iAV_WRITE_DATA;
        6: rREG_FG_VRES <= iAV_WRITE_DATA;
      endcase
    end

    if ((rBG_STATE==stSYNCING)&iBG_SOP&iBG_VALID&&oBG_READY) begin
      rBG_STATE <= stSYNCHED;
    end
    if ((rFG_STATE==stSYNCING)&iFG_SOP&iFG_VALID&&oFG_READY) begin
      rFG_STATE <= stSYNCHED;
    end
    // this is the frame start
    if (rFG_STATE==stSYNCHED&&rBG_STATE==stSYNCHED) begin
      rFG_ENABLED <= rREG_FG_ENABLED;
      rHPOS <= rREG_HPOS;
      rHEND <= rREG_HPOS+rREG_FG_HRES;
      rVPOS <= rREG_VPOS;
      rVEND <= rREG_VPOS+rREG_FG_VRES;
      rBG_HRES <= rREG_BG_HRES;
      rBG_VRES <= rREG_BG_VRES;

      rFG_STATE <= stPAUSE;
      rBG_STATE <= stPAUSE;
      rHCNT_BG <= 0;
      rVCNT_BG <= 0;
      rVPFLAG <= 0;
      if (rREG_VPOS==0) 
        rVPFLAG<= 1;
    end

    if (rBG_STATE==stVIDEO&&iBG_VALID&&oBG_READY) begin
      rHCNT_BG <= rHCNT_BG+1;
      if ((rHCNT_BG+1)==rHPOS && rVPFLAG) begin
        if (rFG_ENABLED) rFG_STATE <= stVIDEO;
      end
      if (rHCNT_BG==rHEND && rFG_STATE==stVIDEO) begin
        rFG_STATE <= stPAUSE;
      end
      if (rHCNT_BG==rBG_HRES-1) begin
        rBG_STATE <= stPAUSE;
        rHCNT_BG <= 0;
        rVCNT_BG <= rVCNT_BG+1;
        rVPFLAG <= rVPFLAG | (rVPOS==rVCNT_BG+1) &!(rVEND == rVCNT_BG+1);
      end
      if ((rHCNT_BG==rBG_HRES-2) &&(rVCNT_BG==rBG_VRES-1)) begin
        rBG_EOP <= 1;
      end
  end

    // this happens on every start of line, including first
    // here we check for the correct state of the foreground layer and set state accordingly
    if (rBG_STATE==stPAUSE) begin
      if (rFG_STATE==stVIDEO) begin
        // in case foreground left border is out of screen
        rHCNT_BG <= rHCNT_BG+1;
        if (rHCNT_BG==rHEND) begin
          rFG_STATE <= stPAUSE;
        end
      end else begin
        rBG_STATE <= stVIDEO;
        if (rHPOS==0 && rVPFLAG && rFG_STATE==stPAUSE)
          rFG_STATE <= stVIDEO;
      end
    end

    if (oFG_READY&&iFG_VALID) begin
      rFG_DATA <= {iFG_SOP, iFG_EOP,iFG_ALPHA,iFG_DATA};
      if (rFG_DATA[cFG_EOP_BIT]) begin // EOP
        rFG_STATE <= stSYNCING;
      end
      if (iFG_SOP) begin //SOP
        rFG_STATE <= stSYNCHED;
      end
    end

    if (oBG_READY&&iBG_VALID) begin
      rBG_DATA <= {iBG_SOP, iBG_EOP,iBG_DATA};
      if (rBG_DATA[cBG_EOP_BIT]) begin // EOP
        rBG_STATE <= stSYNCING;
      end
      if (iBG_SOP) begin //SOP
        rBG_STATE <= stSYNCHED;
      end
    end

    if (iOUT_READY)
      oOUT_VALID <= 0;

    if (iOUT_READY)
      oOUT_EOP <= 0;
      
    if (oBG_READY&&iBG_VALID&&(rBG_STATE==stVIDEO)|(iOUT_READY&rBG_EOP)) begin
      if (rFG_STATE==stVIDEO && rFG_DATA[cFG_ALPHA_BIT])
        oOUT_DATA <= rFG_DATA[pBIT_WIDTH-1:0];
      else
        oOUT_DATA <= rBG_DATA[pBIT_WIDTH-1:0];
      oOUT_VALID <= 1;
      oOUT_SOP <= rBG_DATA[cBG_SOP_BIT];
      oOUT_EOP <= rBG_DATA[cBG_EOP_BIT] | rBG_EOP;
      if(rBG_EOP) begin
        rBG_STATE <= stSYNCING;
        if (rFG_STATE==stVIDEO || rFG_STATE==stPAUSE)
          rFG_STATE <= stSYNCING;
        rBG_EOP <= 0;
      end
    end
  end
end

endmodule

