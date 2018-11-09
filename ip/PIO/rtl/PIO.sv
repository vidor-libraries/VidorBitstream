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

module PIO #(
  parameter pBITS =32,
  parameter pMUX_BITS = 2
) 
(
  input                                 iCLOCK,
  input                                 iRESET,
                           
  input      [cADDRESS_BITS-1:0]                      iADDRESS,
  input                                 iWRITE,
  input                                 iREAD,
  input      [31:0]                     iWRITE_DATA,
  output reg [31:0]                     oREAD_DATA,
  
  input      [pBITS-1:0]                iPIO,
  output reg [pBITS-1:0]                oPIO,
  output reg [pBITS-1:0]                oDIR,
  output reg [pBITS*pMUX_BITS-1:0]      oMUXSEL
);

localparam cADDRESS_BITS = $clog2(4+(pMUX_BITS*pBITS)/32);

always @(posedge iCLOCK)
begin
  if (iWRITE) begin
    case (iADDRESS)
      0: oPIO           <= iWRITE_DATA;
      1: oDIR           <= iWRITE_DATA;
      2: oPIO           <= oPIO&~iWRITE_DATA;
      3: oPIO           <= oPIO|iWRITE_DATA;
      default:
        if (iADDRESS<(4+pMUX_BITS)) begin
		      if (iADDRESS==(4+$clog2(pBITS*pMUX_BITS)))
            oMUXSEL[ pBITS*pMUX_BITS-1 -: ((pBITS*pMUX_BITS-1)%32)+1] <= iWRITE_DATA;
			    else 
            oMUXSEL[ (32*(iADDRESS-3)-1) -:32] <= iWRITE_DATA;
			end
    endcase
  end
  if (iREAD) begin
    case (iADDRESS)
      0: oREAD_DATA <= iPIO;
      1: oREAD_DATA <= oDIR;
      2: oREAD_DATA <= oPIO;
      3: oREAD_DATA <= oPIO;
      default:
        if (iADDRESS<(4+pMUX_BITS)) begin
		      if (iADDRESS==(4+$clog2(pBITS*pMUX_BITS)))
            oREAD_DATA <= oMUXSEL[ pBITS*pMUX_BITS-1 -: ((pBITS*pMUX_BITS-1)%32)+1] ;
			    else 
            oREAD_DATA <= oMUXSEL[ (32*(iADDRESS-3)-1) -:32];
			  end
    endcase
  end
end

endmodule