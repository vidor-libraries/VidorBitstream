# **DVI_OUT** IP core

Produces DVI (TMDS) output using generic pins (set up as LVDS outputs).

## Ports

<table>
    <tr><th          >Signal Group  </th><th>Signal Name</th> <th>Direction</th> <th>Width</th> <th>Description                   </th> </tr>
    <tr><th rowspan=2>Clock/Reset   </th><td>iPCLK      </td> <td>Input    </td> <td>  1  </td> <td>Pixel clock                   </td> </tr>
    <tr>                                 <td>iSCLK      </td> <td>Input    </td> <td>  1  </td> <td>Shift Clock (10x Pixel Clock) </td> </tr>
    <tr><th rowspan=6>Video Port    </th><td>iRED       </td> <td>Input    </td> <td>  8  </td> <td>Red color input               </td> </tr>
    <tr>                                 <td>iGRN       </td> <td>Input    </td> <td>  8  </td> <td>Green color input             </td> </tr>
    <tr>                                 <td>iBLU       </td> <td>Input    </td> <td>  8  </td> <td>Blue color input              </td> </tr>
    <tr>                                 <td>iHS        </td> <td>Input    </td> <td>  1  </td> <td>Horizontal Sync input         </td> </tr>
    <tr>                                 <td>iVS        </td> <td>Input    </td> <td>  1  </td> <td>Vertical Sync input           </td> </tr>
    <tr>                                 <td>iDE        </td> <td>Input    </td> <td>  1  </td> <td>Data Enable input             </td> </tr>
    <tr><th rowspan=2>TMDS Output   </th><td>oDVI_DATA  </td> <td>Output   </td> <td>  3  </td> <td>TMDS Output Data              </td> </tr>
    <tr>                                 <td>oDVI_CLK   </td> <td>Output   </td> <td>  1  </td> <td>TMDS Output Clock             </td> </tr>
</table>

## Parameters

The IP has no parameters

## Description

**DVI_OUT** core allows inputting a video stream and outputting a TMDS formatted stream that can directly be connected to FPGA pins.
Input video stream should be the output of a *CRT Controller* such as **FBST**, that produces red, green and blue color components in addition to timing signals (HS/VS/DE).
Output video stream is a serialization of the input data after being converted to TMDS. for this reason the IP block requires two different clocks, the Pixel clock at pixel rate, and the Shift clock at 10 times the pixel clock rate, used to produce TMDS bits.
TMDS outputs should (data and clock) should be connected to LVDS capable pins and should be capacitively decoupled in order to be compatible with TMDS standard.
