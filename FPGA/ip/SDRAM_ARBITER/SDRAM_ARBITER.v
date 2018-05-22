module SDRAM_ARBITER
#(
parameter pBURST_SIZE=64,
parameter pCAM_OFFSET_A = 0,
parameter pCAM_OFFSET_B = 640*480,
parameter pFB_OFFSET = 2*640*480,
parameter pFB_SIZE = 640*480,
parameter pADDRESS_BITS = 22
)
(
  input                      iFB_CLK,
  input                      iMEM_CLK,
  input                      iMIPI_CLK,
  input                      iRESET,
                             
  output                     oFB_START,
  output [30:0]              oFB_DATA,
  output                     oFB_DATAVALID,
  input                      iFB_READY,

  input [14:0]               iMIPI_DATA,
  input                      iMIPI_DATAVALID,
  input                      iMIPI_START,

  output [pADDRESS_BITS-1:0] oSDRAM_ADDRESS,
  output                     oSDRAM_WRITE,
  output                     oSDRAM_READ,
  output [15:0]              oSDRAM_WRITEDATA,
  input  [15:0]              iSDRAM_READ_DATA,
  input                      iSDRAM_READ_DATAVALID,
  input                      iSDRAM_WAITREQUEST,
  output [1:0]               oSDRAM_BYTEENABLE,

  input [pADDRESS_BITS-1:0]  iAVL_ADDRESS,
  input                      iAVL_READ,
  input                      iAVL_WRITE,
  output                     oAVL_WAITREQUEST,
  output [15:0]              oAVL_READ_DATA,
  output                     oAVL_READ_DATAVALID,
  input  [15:0]              iAVL_WRITE_DATA,
  input  [1:0]               iAVL_BYTEENABLE,
  input  [5:0]               iAVL_BURSTCOUNT
);

  function integer CLogB2;
    input [31:0] Depth;
    integer i;
    begin
      i = Depth-1;        
      for(CLogB2 = 0; i > 0; CLogB2 = CLogB2 + 1)
        i = i >> 1;
    end
  endfunction

  localparam cBURST_BITS = CLogB2(pBURST_SIZE)+1;

wire                       wMIPI_FIFO_FULL;
wire [cBURST_BITS+2+1-1:0] wMIPI_FIFO_USEDW;
wire [15:0]                wMIPI_FIFO_DATA;
reg                        rMIPI_UNLOCK;
wire [cBURST_BITS+2+1-1:0] wFB_FIFO_USEDW;
wire [cBURST_BITS+2+1-1:0] wFB_FIFO2_USEDW;
reg  [cBURST_BITS-1:0]     rBURSTCNT;
wire [cBURST_BITS-1:0]     wBURSTCNT;
reg  [1:0]                 rCURRENT_CLIENT;
reg  [pADDRESS_BITS-1:0]   rREAD_ADDRESS, 
                           rWRITE_ADDRESS, 
                           rADDRESS;
reg                        rWRITE, 
                           rREAD;
reg                        rFB_START;
reg                        rCMD_WRITE, 
                           rCMD_READ;
reg                        rBUFFER;

reg  [1:0]                 rREAD_CLIENT;
wire [1:0]                 wREAD_CLIENT;
reg  [cBURST_BITS-1:0]     rCURRENT_BURSTCNT;
wire                       wFB_START;
reg                        rAVL_ACTIVE;
reg                        rAVL_WAIT;
reg  [1:0]                 rBYTEENABLE;

initial begin
  rMIPI_UNLOCK<=0;
  rBURSTCNT<=0;
  rCURRENT_CLIENT<=0;
  rREAD_ADDRESS<=0;
  rWRITE_ADDRESS<=0;
  rADDRESS<=0;
  rWRITE<=0;
  rREAD<=0;
  rFB_START<=1;
  rCMD_WRITE<=1;
  rCMD_READ<=0;
  rREAD_CLIENT<=0;
  rCURRENT_BURSTCNT<=0;
  rAVL_ACTIVE<=0;
  rAVL_WAIT <=1;
  rBUFFER<=0;
end

  dcfifo #(
    .intended_device_family     ("CYCLONE 10 LP"),
    .lpm_type                   ("dcfifo"),
    .add_usedw_msb_bit          ("ON"),
    .overflow_checking          ("ON"),
    .rdsync_delaypipe           (4),
    .underflow_checking         ("ON"),
    .use_eab                    ("ON"),
    .write_aclr_synch           ("OFF"),
    .add_ram_output_register    ("OFF"),
    .lpm_numwords               (pBURST_SIZE*4),
    .lpm_showahead              ("ON"),
    .lpm_width                  (16),
    .lpm_widthu                 (cBURST_BITS+2),
    .lpm_hint                   ("unused"),     
    .wrsync_delaypipe           (4) 
  )   
  mipi_fifo (
    .aclr         (1'b0),

    .wrclk        (iMIPI_CLK),
    .wrreq        (iMIPI_DATAVALID&!wMIPI_FIFO_FULL),
    .wrempty      (),
    .wrfull       (wMIPI_FIFO_FULL),
    .wrusedw      (),
    .data         ({iMIPI_START,iMIPI_DATA}),
 
    .rdclk        (iMEM_CLK),
    .rdreq        (!rAVL_ACTIVE&rWRITE&!wMIPI_FIFO_DATA[15]&!iSDRAM_WAITREQUEST|rMIPI_UNLOCK),
    .rdempty      (),
    .rdusedw      (wMIPI_FIFO_USEDW),
    .rdfull       (),
    .q            (wMIPI_FIFO_DATA) 
  );

    dcfifo #(
    .intended_device_family     ("CYCLONE 10 LP"),
    .lpm_type                   ("dcfifo"),
    .add_usedw_msb_bit          ("ON"),
    .overflow_checking          ("ON"),
    .rdsync_delaypipe           (4),
    .underflow_checking         ("ON"),
    .use_eab                    ("ON"),
    .write_aclr_synch           ("OFF"),
    .add_ram_output_register    ("OFF"),
    .lpm_numwords               (pBURST_SIZE*4),
    .lpm_showahead              ("ON"),
    .lpm_width                  (16),
    .lpm_widthu                 (cBURST_BITS+2),
    .lpm_hint                   ("unused"),     
    .wrsync_delaypipe           (4) 
  )   
  fb_fifo (
    .aclr         (1'b0),

    .wrclk        (iMEM_CLK),
    .wrreq        ((iSDRAM_READ_DATAVALID&&(rCURRENT_CLIENT==2'd0))|
                   !wCMD_EMPTY&(wFB_START&&(rCURRENT_BURSTCNT==0))),
    .wrempty      (),
    .wrfull       (),
    .wrusedw      (wFB_FIFO_USEDW),
    .data         ({wFB_START&(rCURRENT_BURSTCNT==0),iSDRAM_READ_DATA[14:0]}),
 
    .rdclk        (iFB_CLK),     
    .rdreq        (iFB_READY&!wFB_FIFO2_EMPTY&!wFB_FIFO_EMPTY),
    .rdempty      (wFB_FIFO_EMPTY),
    .rdusedw      (),
    .rdfull       (),
    .q            ({oFB_START,oFB_DATA[14:0]}) 
  );

dcfifo #(
    .intended_device_family     ("CYCLONE 10 LP"),
    .lpm_type                   ("dcfifo"),
    .add_usedw_msb_bit          ("ON"),
    .overflow_checking          ("ON"),
    .rdsync_delaypipe           (4),
    .underflow_checking         ("ON"),
    .use_eab                    ("ON"),
    .write_aclr_synch           ("OFF"),
    .add_ram_output_register    ("OFF"),
    .lpm_numwords               (pBURST_SIZE*4),
    .lpm_showahead              ("ON"),
    .lpm_width                  (16),
    .lpm_widthu                 (cBURST_BITS+2),
    .lpm_hint                   ("unused"),     
    .wrsync_delaypipe           (4) 
  )   
  fb_fifo2 (
    .aclr         (1'b0),

    .wrclk        (iMEM_CLK),
    .wrreq        ((iSDRAM_READ_DATAVALID&&(rCURRENT_CLIENT==2'd1))|
                   !wCMD_EMPTY&(wFB_START&&(rCURRENT_BURSTCNT==0))),
    .wrempty      (),
    .wrfull       (),
    .wrusedw      (),
    .data         (iSDRAM_READ_DATA),
 
    .rdclk        (iFB_CLK),
    .rdreq        (iFB_READY&!wFB_FIFO2_EMPTY&!wFB_FIFO_EMPTY),
    .rdempty      (wFB_FIFO2_EMPTY),
    .rdusedw      (wFB_FIFO2_USEDW),
    .rdfull       (),
    .q            (oFB_DATA[30:15]) 
  );

scfifo #(
    .intended_device_family     ("CYCLONE 10 LP"),
    .lpm_type                   ("scfifo"),
    .overflow_checking          ("ON"),
    .underflow_checking         ("ON"),
    .use_eab                    ("OFF"),
    .add_ram_output_register    ("OFF"),
    .lpm_numwords               (8),
    .lpm_showahead              ("ON"),
    .lpm_width                  (cBURST_BITS+3),
    .lpm_widthu                 (3),
    .lpm_hint                   ("unused")
)   
  cmd_fifo (
    .aclr         (1'b0),

    .clock        (iMEM_CLK),
    .wrreq        (rCMD_WRITE),
    .data         ({rFB_START&!rREAD,rREAD_CLIENT,rBURSTCNT}),
 
    .rdreq        (((wFB_START&&rCURRENT_BURSTCNT==0)|
                   (rCURRENT_BURSTCNT==0)|
                   (rCURRENT_BURSTCNT==1)&iSDRAM_READ_DATAVALID&!wFB_START)&!wCMD_EMPTY),
    .empty        (wCMD_EMPTY),
    .q            ({wFB_START,wREAD_CLIENT, wBURSTCNT}) 
  );

assign oFB_DATAVALID=!wFB_FIFO_EMPTY&!wFB_FIFO2_EMPTY&&(wFB_FIFO2_USEDW>pBURST_SIZE/3);

assign oAVL_WAITREQUEST=rAVL_WAIT;
assign oAVL_READ_DATA = iSDRAM_READ_DATA;
assign oAVL_READ_DATAVALID = iSDRAM_READ_DATAVALID&&(rCURRENT_CLIENT==2'd2);

always @(posedge iMEM_CLK)
begin
  if (iRESET) begin
    rBURSTCNT<=0;
    rREAD<=0;
    rWRITE <=0;
    rREAD_ADDRESS<=pCAM_OFFSET_A;
    rWRITE_ADDRESS<=pCAM_OFFSET_B;
    rADDRESS<=0;
    rCURRENT_BURSTCNT<=0;
    rAVL_WAIT <=1;
    rBUFFER<=0;
  end else begin
    rCMD_WRITE<=0;
    rCMD_READ<=0;
    rAVL_WAIT<=1;

    if (rCMD_WRITE&!rREAD&rFB_START) begin
      rFB_START<=0;
    end
    rMIPI_UNLOCK <=0; 
      
    if (!rWRITE&&!rREAD) begin
      // no operation is ongoing...

      // check if MIPI fifo has start bit. if so reset address, change buffer and drop that bit
      if (wMIPI_FIFO_DATA[15]&!rMIPI_UNLOCK) begin
        rWRITE_ADDRESS<=0;
        rBUFFER<=!rBUFFER;
        rMIPI_UNLOCK <=1;
      end
      // check if there is enough data from camera
      else if (wMIPI_FIFO_USEDW>pBURST_SIZE) begin
        rWRITE<=1;
        rBYTEENABLE<=2'd3;
        rBURSTCNT <=pBURST_SIZE-1;
        rADDRESS<=rWRITE_ADDRESS+(rBUFFER ? pCAM_OFFSET_A : pCAM_OFFSET_B);
      end
      // else check if there is enough room in FB FIFO
      else if (wFB_FIFO_USEDW<pBURST_SIZE) begin
        rREAD<=1;
        rBYTEENABLE<=2'd3;
        rCMD_WRITE<=1;
        rREAD_CLIENT<=0;
        rBURSTCNT <=pBURST_SIZE-1;
        rADDRESS<=rREAD_ADDRESS+(rBUFFER ? pCAM_OFFSET_B : pCAM_OFFSET_A);
      end
      else if (iAVL_READ) begin
        rREAD <=1;
        rBYTEENABLE<=iAVL_BYTEENABLE;
        rCMD_WRITE<=1;
        rREAD_CLIENT<=2;
        rBURSTCNT <= 0;//iAVL_BURSTCOUNT-1;
        rADDRESS<=iAVL_ADDRESS;
        rAVL_WAIT<=0;
      end
      else if (iAVL_WRITE&&(iAVL_BYTEENABLE!=0)) begin
        rWRITE<=1;
        rBURSTCNT <= 0;//iAVL_BURSTCOUNT-1;
        rADDRESS<=iAVL_ADDRESS;
        rAVL_ACTIVE<=1;
        rAVL_WAIT<=0;
        rBYTEENABLE<=iAVL_BYTEENABLE;
      end
    end
    
    if (rWRITE&&!iSDRAM_WAITREQUEST)
    begin
      rAVL_WAIT <= !rAVL_ACTIVE;
      rBURSTCNT<=rBURSTCNT-1;
      if (!rAVL_ACTIVE) 
        rWRITE_ADDRESS<=rWRITE_ADDRESS+1;
      rADDRESS<=rADDRESS+1;
      if (rWRITE_ADDRESS==(pFB_SIZE-1)) begin
        //rBUFFER<=!rBUFFER;
        rWRITE_ADDRESS<=0;
      end
      if (rBURSTCNT==0||wMIPI_FIFO_DATA[15]) begin
        rWRITE <=0;
        rAVL_ACTIVE<=0;
        rAVL_WAIT<=1;
      end
    end

    // generate read requests until all burst has been complete.
    if (rREAD&&!iSDRAM_WAITREQUEST)
    begin
      rBURSTCNT<=rBURSTCNT-1;
      rADDRESS<=rADDRESS+1;
      if (rBURSTCNT==0) begin
        if (rREAD_CLIENT==0) begin
          rCMD_WRITE<=1;
          rREAD_CLIENT<=1;
          rBURSTCNT <=pBURST_SIZE-1;
          rADDRESS<=rREAD_ADDRESS+pFB_OFFSET;
          rREAD_ADDRESS<=rREAD_ADDRESS+pBURST_SIZE;
          if (rREAD_ADDRESS==(pFB_SIZE-pBURST_SIZE)) begin
            rREAD_ADDRESS<=0;
            rFB_START <=1;
          end
        end 
        else begin
          rREAD <=0;
          if (rFB_START)
            rCMD_WRITE <=1;
        end
      end
    end
    if (rCURRENT_BURSTCNT&&iSDRAM_READ_DATAVALID)
      rCURRENT_BURSTCNT<=rCURRENT_BURSTCNT-1;
    if ( (rCURRENT_BURSTCNT==0)&!wCMD_EMPTY||
         (rCURRENT_BURSTCNT==1)&iSDRAM_READ_DATAVALID&!wCMD_EMPTY) begin
      rCURRENT_BURSTCNT<= wFB_START ? 0 : {1'b0,wBURSTCNT}+1;
      rCURRENT_CLIENT<= wREAD_CLIENT;
    end
  end
end

assign oSDRAM_ADDRESS    = rADDRESS;
assign oSDRAM_READ       = rREAD;
assign oSDRAM_WRITE      = rWRITE;
assign oSDRAM_BYTEENABLE = rBYTEENABLE;
assign oSDRAM_WRITEDATA  = rAVL_ACTIVE ? iAVL_WRITE_DATA : wMIPI_FIFO_DATA[14:0];

endmodule
