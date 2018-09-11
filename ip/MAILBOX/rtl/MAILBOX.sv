/*
* Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
* This file is part of Vidor IP.
* Copyright (c) 2018
* Authors: Dario Pennisi
*
* This software is released under:
* The GNU General Public License, which covers the main part of 
* Vidor IP
* The terms of this license can be found at:
* https://www.gnu.org/licenses/gpl-3.0.en.html
*
* You can be released from the requirements of the above licenses by purchasing
* a commercial license. Buying such a license is mandatory if you want to modify or
* otherwise use the software for commercial activities involving the Arduino
* software without disclosing the source code of your own applications. To purchase
* a commercial license, send an email to license@arduino.cc.
*
*/

module MAILBOX #(
  parameter pDPRAM_BITS=10,
  parameter pFIFO_BITS =4
) (
  input              iCLOCK,
  input              iRESET,

  input       [pDPRAM_BITS-1:0] iMST_ADDRESS,
  input              iMST_WRITE,
  input              iMST_READ,
  input       [31:0] iMST_WRITE_DATA,
  output  reg [31:0] oMST_READ_DATA,

  input       [pDPRAM_BITS-1:0] iSLV_ADDRESS,
  input              iSLV_WRITE,
  input              iSLV_READ,
  input       [31:0] iSLV_WRITE_DATA,
  output reg  [31:0] oSLV_READ_DATA,

  output reg         oMST_AK,
  input              iMST_RQ
);

localparam cFIFO_START_ADDR = (1<<(pDPRAM_BITS))-(1<<(pFIFO_BITS));
localparam cLAST_ADDRESS    = (1<<(pDPRAM_BITS))-1;

reg [31:0] rDPRAM [cLAST_ADDRESS:0];
reg [pFIFO_BITS-1:0]  rRD_PTR;
reg [pFIFO_BITS-1:0]  rWR_PTR;

reg [2:0]  rRESYNC_MST_RQ;
reg        rPERSIST_MST_RQ;
reg        rRET_AVAIL;

wire unsigned [pFIFO_BITS-1:0] wRD_USED;
wire unsigned [pFIFO_BITS-1:0] wWR_FREE;

assign wRD_USED = rWR_PTR-rRD_PTR;
assign wWR_FREE = (1<<pFIFO_BITS)-1-rWR_PTR+rRD_PTR;

assign oMST_AK = rRET_AVAIL || (wRD_USED>0);

always @(posedge iCLOCK) begin
  if (iRESET) begin
    rRET_AVAIL<= 0;
    rRD_PTR<=0;
    rWR_PTR<=0;
    rPERSIST_MST_RQ<=0;
    rRESYNC_MST_RQ<=0;
  end
  else begin
    rRESYNC_MST_RQ <= {rRESYNC_MST_RQ[1:0],iMST_RQ};
    
    if (rRESYNC_MST_RQ[2])
      rPERSIST_MST_RQ <= rPERSIST_MST_RQ|rRESYNC_MST_RQ[2];

    if (iMST_ADDRESS<cFIFO_START_ADDR) begin
      if (iMST_WRITE) begin
        rDPRAM[iMST_ADDRESS] <= iMST_WRITE_DATA;
      end 
      else if (iMST_READ) begin
        oMST_READ_DATA <= rDPRAM[iMST_ADDRESS];
        if (iMST_ADDRESS==0) begin
          rRET_AVAIL<=0;
        end
      end  
    end else begin
      if (iMST_READ) begin
        if (iMST_ADDRESS==cLAST_ADDRESS) begin
          oMST_READ_DATA <= {rRET_AVAIL,wRD_USED};
        end
        else if (wRD_USED>0) begin
          oMST_READ_DATA <= rDPRAM[rRD_PTR+cFIFO_START_ADDR];
          rRD_PTR <= rRD_PTR+1;
        end else begin
          oMST_READ_DATA <= 32'hDEADBEEF;
        end
      end
    end

    if (iSLV_ADDRESS<cFIFO_START_ADDR) begin
      if (iSLV_WRITE) begin
        rDPRAM[iSLV_ADDRESS] <= iSLV_WRITE_DATA;
        if (iSLV_ADDRESS==0) begin
          rPERSIST_MST_RQ<=0;
          rRET_AVAIL<=1;
        end
      end
      else if (iSLV_READ) begin
        oSLV_READ_DATA <= rDPRAM[iSLV_ADDRESS];
      end
    end else begin
      if (iSLV_READ) begin
        if (iSLV_ADDRESS==cFIFO_START_ADDR) begin
          oSLV_READ_DATA <= wWR_FREE;
        end
      end
      if (iSLV_WRITE) begin
        if (wWR_FREE>0) begin
          rDPRAM[rWR_PTR+cFIFO_START_ADDR] <= iSLV_WRITE_DATA;
          rWR_PTR <= rWR_PTR+1;
        end
      end
    end
  end
end

endmodule
