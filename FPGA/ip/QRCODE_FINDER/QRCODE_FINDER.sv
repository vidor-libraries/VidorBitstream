module QRCODE_FINDER (
input             iCLK,
input             iRESET,

input             iVID_CLK,
input             iVID_RESET,
input [23:0]      iVID_DATA,
input             iVID_START,
input             iVID_DATAVALID,

input [3:0]       iADDRESS,
input [31:0]      iWRITEDATA,
input             iWRITE,
input             iREAD,
output reg [31:0] oREADDATA,

output reg [23:0] oVID_DATA,
output            oVID_START,
output            oVID_DATAVALID
);

parameter pHRES=640;
parameter pVRES=480;
parameter pTHRESH_MIN=4;
parameter pFIXED=5;
parameter pTHRESHOLD=60;
localparam cPIPE_DEPTH=4;

reg [9:0] rHCNT;
reg [9:0] rVCNT /*synthesis noprune */;
reg [cPIPE_DEPTH-1:0] rVALID_PIPE;
reg [cPIPE_DEPTH-1:0] rSTART_PIPE;
reg [cPIPE_DEPTH-1:0] rDSTART_PIPE;
reg [cPIPE_DEPTH-1:0][23:0] rDATA_PIPE;
reg [cPIPE_DEPTH-1:0][9:0] rHCNT_PIPE;
reg rDSTART;
reg [9:0] rLUM;
wire [9:0] wLUM;
wire [9:0] wLUM_NORM;
reg [cPIPE_DEPTH-1:0][9:0] rLUM_HIST;
reg [1:0] rSTATE;
reg [9:0] rPREV_CHANGE;
reg [1:0] rPREV_STATE;
reg [9:0] rPREV_HSTART;
reg [9:0] rPREV_CENT_HSTART;
reg [9:0] rPREV_CENT_HEND;
reg [9:0] rPREV_DUR;
reg [3:0] rDECODE_STATE;
reg [9:0] rREFBIT_DUR_MAX;
reg [9:0] rREFBIT_DUR_MIN;
reg [9:0] rREFBIT_3DUR_MAX;
reg [9:0] rREFBIT_3DUR_MIN;
reg [9:0] rREFBIT_HSTART;
reg rFOUND;
reg rHF1, rHF2;
reg rCHANGE;
reg [9:0] rHSTART;
reg [9:0] rHST1 /*synthesis noprune */, rHED1/*synthesis noprune */, rHST2/*synthesis noprune */, rHED2/*synthesis noprune */, rHST3/*synthesis noprune */, rHED3/*synthesis noprune */;
reg [9:0] rVST1 /*synthesis noprune */, rVED1/*synthesis noprune */, rVST2/*synthesis noprune */, rVED2/*synthesis noprune */, rVST3/*synthesis noprune */, rVED3/*synthesis noprune */;
reg       rFND1/*synthesis noprune */,rFND2/*synthesis noprune */,rFND3/*synthesis noprune */;
reg       rOPN1/*synthesis noprune */,rOPN2/*synthesis noprune */,rOPN3/*synthesis noprune */;

reg [9:0] rREG_HST1 /*synthesis noprune */, rREG_HED1/*synthesis noprune */, rREG_HST2/*synthesis noprune */, rREG_HED2/*synthesis noprune */, rREG_HST3/*synthesis noprune */, rREG_HED3/*synthesis noprune */;
reg [9:0] rREG_VST1 /*synthesis noprune */, rREG_VED1/*synthesis noprune */, rREG_VST2/*synthesis noprune */, rREG_VED2/*synthesis noprune */, rREG_VST3/*synthesis noprune */, rREG_VED3/*synthesis noprune */;
reg       rREG_FND1/*synthesis noprune */,rREG_FND2/*synthesis noprune */,rREG_FND3/*synthesis noprune */;
reg rUPDATE, rREFRESH;
wire signed [12:0] wLUM_DIFF;

assign wLUM_DIFF = {3'b0,rLUM_HIST[0]}+{3'b0,rLUM_HIST[1]}-{3'b0,rLUM_HIST[2]}-{3'b0,rLUM_HIST[3]};


typedef enum {
  stNONE,
  stWHITE,
  stBLACK} eSTATES;

typedef enum {
  dsIDLE,
  dsFIRST,
  dsSECOND,
  dsCENTER,
  dsTHIRD,
  dsFOURTH} eDECODE_STATES;
  
// convert from RGB 8 bit to luminance 10 bit
assign wLUM = {2'b0,iVID_DATA[7:0]}+
              {2'b0,iVID_DATA[15:8]}+
              {2'b0,iVID_DATA[23:16]};

// normalize so that we have full scale on 10 bit
assign wLUM_NORM = rLUM+rLUM[9:2]+rLUM[9:4]+rLUM[9:6]+rLUM[9:8];
assign oVID_START=rSTART_PIPE[2];
assign oVID_DATAVALID=rVALID_PIPE[2];

reg [1:0] rOUTMODE;
always @(posedge iCLK)
begin
  if (iWRITE) begin
    case (iADDRESS)
      0: {rOUTMODE,rREFRESH}<= iWRITEDATA;
    endcase
  end
  if (iREAD) begin
    case (iADDRESS)
      0 : oREADDATA<= {rOUTMODE,rUPDATE}; //control
      1 : oREADDATA<= {rREG_FND3,rREG_FND2,rREG_FND1};
      2 : oREADDATA<= rREG_HST1;
      6 : oREADDATA<= rREG_HST2;
      10: oREADDATA<= rREG_HST3;
      3 : oREADDATA<= rREG_HED1;
      7 : oREADDATA<= rREG_HED2;
      11: oREADDATA<= rREG_HED3;
      4 : oREADDATA<= rREG_VST1;
      8 : oREADDATA<= rREG_VST2;
      12: oREADDATA<= rREG_VST3;
      5 : oREADDATA<= rREG_VED1;
      9 : oREADDATA<= rREG_VED2;
      13: oREADDATA<= rREG_VED3;
      default: oREADDATA<=0;
    endcase
  end
  if (rREFRESH&!rUPDATE)
    rREFRESH<=0;
end

always @(posedge iVID_CLK)
begin
  if (rUPDATE&&rREFRESH)
    rUPDATE<=0;
  rDATA_PIPE  <= {rDATA_PIPE,iVID_DATA};
  rVALID_PIPE <= {rVALID_PIPE,iVID_DATAVALID};
  rSTART_PIPE <= {rSTART_PIPE,iVID_START};
  rDSTART_PIPE <= {rDSTART_PIPE,rDSTART};
  rHCNT_PIPE <= {rHCNT_PIPE,rHCNT};
  //first pipeline stage
  if (iVID_DATAVALID) begin
    rDSTART <= iVID_START;
    rLUM    <= wLUM;
    rHCNT   <= rHCNT+1;
    if (rHCNT==pHRES-1) begin
      rHCNT<=0;
      rSTATE<=stNONE;
      rVCNT<=rVCNT+1;
      rDSTART_PIPE[0]<=1;
    end
    if (rDSTART) begin
      rHCNT<=0;
      rVCNT<=0;
      rOPN1<=0;
      rOPN2<=0;
      rOPN3<=0;
      rFND1<=0;
      rFND2<=0;
      rFND3<=0;
      
      if (!rUPDATE) begin
        rREG_FND1<=rFND1;
        rREG_FND2<=rFND2;
        rREG_FND3<=rFND3;
        rREG_HST1<=rFND1 ? rHST1 : 0;
        rREG_HST2<=rFND2 ? rHST2 : 0;
        rREG_HST3<=rFND3 ? rHST3 : 0;
        rREG_HED1<=rFND1 ? rHED1 : 0;
        rREG_HED2<=rFND2 ? rHED2 : 0;
        rREG_HED3<=rFND3 ? rHED3 : 0;
        rREG_VST1<=rFND1 ? rVST1 : 0;
        rREG_VST2<=rFND2 ? rVST2 : 0;
        rREG_VST3<=rFND3 ? rVST3 : 0;
        rREG_VED1<=rFND1 ? rVED1 : 0;
        rREG_VED2<=rFND2 ? rVED2 : 0;
        rREG_VED3<=rFND3 ? rVED3 : 0;
        rUPDATE <=1;
      end
      rSTATE<=stNONE;
    end
  end
  //second pipeline stage
  if (rVALID_PIPE[0]) begin
    rLUM_HIST<= {rLUM_HIST,wLUM_NORM};
    if (rDSTART_PIPE[0]) begin
      rLUM_HIST<={cPIPE_DEPTH-1{wLUM_NORM}};
    end
  end
  //third pipeline stage
  if (rVALID_PIPE[1]) begin
    rCHANGE<=0;
    oVID_DATA<= rOUTMODE==0 ? rDATA_PIPE[1] :
                rOUTMODE==1 ? {3{rLUM_HIST[0][9:2]}} :
                rOUTMODE==2 ? (rSTATE==stNONE) ? {3{8'h80}} : 
                              (rSTATE==stBLACK) ? {3{8'h0}} : {3{8'hFF}}  :
                              0;

    //  check for rising edge
    if ((wLUM_DIFF>0) && (wLUM_DIFF > pTHRESHOLD ) ) begin
      if ((rSTATE==stWHITE && rPREV_CHANGE<wLUM_DIFF) || 
          (rSTATE!=stWHITE) ) begin
        rHSTART<= rHCNT_PIPE[1];
        rPREV_CHANGE<= wLUM_DIFF;
        if (rSTATE!=stWHITE) begin
          rSTATE <= stWHITE;
          rPREV_STATE <= rSTATE;
          rPREV_HSTART <= rHSTART;
          rCHANGE<=1;
          if (rOUTMODE!=0) oVID_DATA<= 24'hff0000;
          rPREV_DUR<=rHCNT_PIPE[0]-rHSTART;
        end
      end
    end

    //  check for falling edge
    if ((wLUM_DIFF<0) && ((-wLUM_DIFF) > pTHRESHOLD ) ) begin
      if ((rSTATE==stBLACK && rPREV_CHANGE<(-wLUM_DIFF)) || 
          (rSTATE!=stBLACK) ) begin
        rHSTART<= rHCNT_PIPE[1];
        rPREV_CHANGE<= -wLUM_DIFF;
        if (rSTATE!=stBLACK) begin
          rSTATE <= stBLACK;
          rPREV_STATE <= rSTATE;
          rPREV_HSTART <= rHSTART;
          rCHANGE<=1;
          if (rOUTMODE!=0) oVID_DATA<= 24'h0000ff;
          rPREV_DUR<=rHCNT_PIPE[0]-rHSTART;
        end
      end
    end
    if (rDSTART_PIPE[1]) begin
      rPREV_STATE<={2{stNONE}};
      rHSTART <=0;
      rCHANGE<=0;
    end
    rFOUND<=0;
    if (rFOUND && rOUTMODE!=0)
      oVID_DATA<= 24'h00FF00;
  end
  //fourth pipeline stage
  if (rVALID_PIPE[2]) begin
    if (rDSTART_PIPE[2])
      rDECODE_STATE<=dsIDLE;
    else if (rCHANGE) begin
      case (rDECODE_STATE)
        dsIDLE: begin
          if (rPREV_STATE==stBLACK) begin
            rREFBIT_DUR_MAX <=rPREV_DUR+(rPREV_DUR>>2)+1;
            rREFBIT_DUR_MIN <=rPREV_DUR-(rPREV_DUR>>2)-1;
            rREFBIT_3DUR_MAX <=(rPREV_DUR<<1)+rPREV_DUR+(rPREV_DUR>>3)+(rPREV_DUR>>2)+3;
            rREFBIT_3DUR_MIN <=(rPREV_DUR<<1)+rPREV_DUR-(rPREV_DUR>>3)-(rPREV_DUR>>2)-3;
            rDECODE_STATE<=dsFIRST;
            rREFBIT_HSTART<= rPREV_HSTART;
          end
        end
        dsFIRST: begin
//          if (rPREV_STATE==stWHITE) begin
          if ((rREFBIT_DUR_MAX>=rPREV_DUR) && 
              (rREFBIT_DUR_MIN<=rPREV_DUR)  ) begin
            rDECODE_STATE<= dsCENTER;
          end
          else 
            rDECODE_STATE<= dsIDLE;
//          end
        end
        dsCENTER: begin
          if ((rREFBIT_3DUR_MAX>=rPREV_DUR) && 
              (rREFBIT_3DUR_MIN<=rPREV_DUR)  ) begin
            rDECODE_STATE<= dsTHIRD;
            rPREV_CENT_HSTART<= rPREV_HSTART;
          end
          else begin
            rREFBIT_DUR_MAX <=rPREV_DUR+(rPREV_DUR>>2)+1;
            rREFBIT_DUR_MIN <=rPREV_DUR-(rPREV_DUR>>2)-1;
            rREFBIT_3DUR_MAX <=(rPREV_DUR<<1)+rPREV_DUR+(rPREV_DUR>>3)+(rPREV_DUR>>2)+3;
            rREFBIT_3DUR_MIN <=(rPREV_DUR<<1)+rPREV_DUR-(rPREV_DUR>>3)-(rPREV_DUR>>2)-3;
            rDECODE_STATE<=dsFIRST;
            rREFBIT_HSTART<= rPREV_HSTART;
          end
        end
        dsTHIRD: begin
          if ((rREFBIT_DUR_MAX>=rPREV_DUR) && 
              (rREFBIT_DUR_MIN<=rPREV_DUR)  ) begin
            rDECODE_STATE<= dsFOURTH;
            rPREV_CENT_HEND<= rPREV_HSTART;
          end
          else 
            rDECODE_STATE<= dsIDLE;
        end
        dsFOURTH: begin
          if ((rREFBIT_DUR_MAX>=rPREV_DUR) && 
              (rREFBIT_DUR_MIN<=rPREV_DUR)  ) begin
            rDECODE_STATE<= dsIDLE;
            // pattern found!!!
            rFOUND<=1;
            if ((rVCNT-rVED1)>10) begin
              rOPN1<=0;
              if ((rVED1-rVST1)<3) rFND1 <=0;
            end
            if ((rVCNT-rVED2)>10) begin
              rOPN2<=0;
              if ((rVED2-rVST2)<3) rFND2 <=0;
            end
            if ((rVCNT-rVED3)>10) begin
              rOPN3<=0;
              if ((rVED3-rVST3)<3) rFND3 <=0;
            end
            if (!rFND1) begin
              rFND1<=1;
              rHST1<=rPREV_CENT_HSTART;
              rHED1<=rPREV_CENT_HEND;
              rVST1<=rVCNT;
              rVED1<=rVCNT;
              rOPN1<=1;
            end
            else if ( rOPN1 && 
                      {rHST1[9:1]+rHED1[9:1]}>rREFBIT_HSTART &&
                      {rHST1[9:1]+rHED1[9:1]}<rHCNT_PIPE[1] ) begin
                rVED1<=rVCNT;
            end
            else if (!rFND2 && ((rPREV_CENT_HEND-rPREV_CENT_HSTART)>((rHED1-rHST1)/2))) begin
              rFND2<=1;
              rHST2<=rPREV_CENT_HSTART;
              rHED2<=rPREV_CENT_HEND;
              rVST2<=rVCNT;
              rVED2<=rVCNT;
              rOPN2<=1;
            end
            else if (rOPN2 &&
                    {rHST2[9:1]+rHED2[9:1]}>rREFBIT_HSTART &&
                    {rHST2[9:1]+rHED2[9:1]}<rHCNT_PIPE[1] ) begin
                rVED2<=rVCNT;
            end
            else if (!rFND3 && ((rPREV_CENT_HEND-rPREV_CENT_HSTART)>((rHED1-rHST1)/2))) begin
              rFND3<=1;
              rHST3<=rPREV_CENT_HSTART;
              rHED3<=rPREV_CENT_HEND;
              rVST3<=rVCNT;
              rVED3<=rVCNT;
              rOPN3<=1;
            end
            else if (rOPN3 &&
                    {rHST3[9:1]+rHED2[9:1]}>rREFBIT_HSTART &&
                    {rHST3[9:1]+rHED2[9:1]}<rHCNT_PIPE[1] ) begin
                rVED3<=rVCNT;
            end
          end
          else begin
            rREFBIT_DUR_MAX <=rPREV_DUR+(rPREV_DUR>>2)+1;
            rREFBIT_DUR_MIN <=rPREV_DUR-(rPREV_DUR>>2)-1;
            rREFBIT_3DUR_MAX <=(rPREV_DUR<<1)+rPREV_DUR+(rPREV_DUR>>3)+(rPREV_DUR>>2)+3;
            rREFBIT_3DUR_MIN <=(rPREV_DUR<<1)+rPREV_DUR-(rPREV_DUR>>3)-(rPREV_DUR>>2)-3;
            rDECODE_STATE<=dsFIRST;
            rREFBIT_HSTART<= rPREV_HSTART;
          end
        end
      endcase
    end
  end
end
endmodule

/*
reg  [9:0] rLUM_AVG[3:0];
reg [9:0] rHCNT;
reg [9:0] rVCNT;
reg [9:0] rLUM_1;
wire [9:0] wLUM;
reg rDSTART;
reg signed [10:0] rDIF1[1:0], rDIF2[1:0];
reg [10:0] rTHRESHOLD;
reg signed [10:0] rY1SIGN,rEDGE_Y1SIGN;
reg [9+pFIXED:0] rEDGE, rLAST_EDGE;
reg [9+pFIXED:0] rPREV_EDGE;
reg [3:0] rVALID_PIPE;
reg [3:0] rSTART_PIPE;
reg [3:0][23:0] rDATA_PIPE;
reg rEDGE_PROCESS;

// convert from RGB 8 bit to luminance 10 bit
assign wLUM = {2'b0,iVID_DATA[7:0]}+
              {2'b0,iVID_DATA[15:8]}+
              {2'b0,iVID_DATA[23:16]};

function [9:0] abs(input [9:0] val);
  abs = (val[8]) ? -val : val;
endfunction

always @(posedge iVID_CLK)
begin
  rVALID_PIPE <= {rVALID_PIPE,iVID_DATAVALID};
  rSTART_PIPE <= {rSTART_PIPE,iVID_START};
  rDATA_PIPE <= {rDATA_PIPE[3:1],{3{rLUM_AVG[0][9:2]}},iVID_DATA};
  // clock when iSTART is high does not carry valid information
  // so we need to delay it
  if (iVID_DATAVALID) begin
    rEDGE_PROCESS<=0;
    rDSTART<=iVID_START;
    if (rDSTART)
    begin
      rTHRESHOLD <= pTHRESH_MIN;
      rY1SIGN<=0;
      rPREV_EDGE <=0;
      rHCNT<=0;
      rVCNT<=0;
      rLUM_AVG[0]=wLUM;
      rLUM_AVG[1]=wLUM;
      rLUM_AVG[2]=wLUM;
      rLUM_AVG[3]=wLUM;
    end
    else begin
      rHCNT<=rHCNT+1;
      
      rLUM_AVG[0]=rLUM_AVG[0][9:1]+rLUM_AVG[0][9:2]+wLUM[9:1];
      rLUM_AVG[1]=rLUM_AVG[0];
      rLUM_AVG[2]=rLUM_AVG[1];
      rLUM_AVG[3]=rLUM_AVG[2];

      if (rHCNT==pHRES-1) begin
        rHCNT<=0;
        rVCNT<=rVCNT+1;
        rLUM_AVG[0]=wLUM;
        rLUM_AVG[1]=wLUM;
        rLUM_AVG[2]=wLUM;
        rLUM_AVG[3]=wLUM;
      end
    end
  end
  if (rVALID_PIPE[0]) begin
    // first pipeline stage
    rDIF1[1]<= rLUM_AVG[2]-rLUM_AVG[3];
    if ((abs(rLUM_AVG[1]-rLUM_AVG[2])<abs(rLUM_AVG[2]-rLUM_AVG[3])) &&
        ((rLUM_AVG[1]-rLUM_AVG[2])>0)==((rLUM_AVG[2]-rLUM_AVG[3])>0) )
      rDIF1[0]<= rLUM_AVG[2]-rLUM_AVG[3];
    else
      rDIF1[0]<= rLUM_AVG[1]-rLUM_AVG[2];
    
    rDIF2[0] <= {1'b0,rLUM_AVG[0]}-{rLUM_AVG[1],1'b0}+{1'b0,rLUM_AVG[2]};
    rDIF2[1] <= {1'b0,rLUM_AVG[1]}-{rLUM_AVG[2],1'b0}+{1'b0,rLUM_AVG[3]};
    
    if ((rTHRESHOLD-(rTHRESHOLD>>2))>pTHRESH_MIN)
      rTHRESHOLD<=rTHRESHOLD-(rTHRESHOLD>>2);
    else rTHRESHOLD<=pTHRESH_MIN;
  end
  if (rVALID_PIPE[1]) begin
    rEDGE_PROCESS<=0;
    // second pipeline stage
    if ((rDIF2[0]==0 || (rDIF2[0][10]^rDIF2[1][10]>0)) && (abs(rDIF1[0])>=rTHRESHOLD))
    begin
      if (rY1SIGN[10] ^ rDIF1[0][10])
      begin
        // edge process
        if (rY1SIGN==0)
        begin
          rLAST_EDGE <=1<<pFIXED;
          rEDGE<=1<<pFIXED;
        end
        else if (rLAST_EDGE==0)
          rLAST_EDGE<=rEDGE;
        rEDGE_PROCESS<=1;
        rEDGE_Y1SIGN <= -rY1SIGN;
      end
      if ((rY1SIGN[10] ^ rDIF1[0][10]) || (abs(rY1SIGN) < abs(rDIF1[0]) ))
      begin
        rY1SIGN <= rDIF1[0];
        if ((rDIF1[0][10:1]+rDIF1[0][10:2]+rDIF1[0][10:3])<pTHRESH_MIN)
          rTHRESHOLD<= pTHRESH_MIN;
        else 
          rTHRESHOLD<= rDIF1[0][10:1]+rDIF1[0][10:2]+rDIF1[0][10:3];
        //if ((rDIF2[0]-rDIF2[1])==0)
          rEDGE <= (rHCNT)<<(pFIXED-1);
        //else if (rDIF2[0])
        //  rEDGE <= (rHCNT<<pFIXED) - ((rDIF2[0]<<pFIXED)+1)/(rDIF2[0]-rDIF2[1]);
      end
      
    end
  end
  if (rVALID_PIPE[2]) begin
    // third pipeline stage
    if (rEDGE_PROCESS) begin
      rLAST_EDGE<=rEDGE;
      //rDIFF <= rEDGE-rLAST_EDGE;
      if (rEDGE_Y1SIGN[10])
        rDATA_PIPE[3]<=24'h0000ff;
      else
        rDATA_PIPE[3]<=24'h00ff00;
    end
  end
  // 
end

assign oVID_DATA=rDATA_PIPE[3];
assign oVID_START=rSTART_PIPE[3];
assign oVID_DATAVALID=rVALID_PIPE[3];

endmodule
*/