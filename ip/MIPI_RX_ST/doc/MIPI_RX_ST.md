# **MIPI_RX_ST** IP core

MIPI CSI2 Receiver.

## Ports

<table>
    <tr><th          >Signal Group    </th><th>Signal Name     </th> <th>Direction</th> <th>Width</th> <th>Description                   </th> </tr>
    <tr><th rowspan=2>MIPI CSI2 input </th><td>iMIPI_D         </td> <td>Input    </td> <td>  2  </td> <td>MIPI Data lanes               </td> </tr>
    <tr>                                   <td>iMIPI_CLK       </td> <td>Input    </td> <td>  1  </td> <td>MIPI Clock lane               </td> </tr>
    <tr><th rowspan=3>Avalon Master   </th><td>oMIPI_DATA      </td> <td>Output   </td> <td> 24  </td> <td>RGB Data                      </td> </tr>
    <tr>                                   <td>oMIPI_START     </td> <td>Output   </td> <td>  1  </td> <td>Frame start flag              </td> </tr>
    <tr>                                   <td>oMIPI_DATA_VALID</td> <td>Output   </td> <td>  1  </td> <td>Data valid flag               </td> </tr>
</table>

## Parameters

The IP has no parameters

## Description

**MIPI_RX_ST** IP extracts 8 bit raw (bayer encoded) data from MIPI CSI2 and outputs a video stream containing 4:4:4 RGB video.

Video data is extracted exclusively from cMIPI_HT_RAW8 (decimal 42) packets and is converted from bayer to 4:4:4: RGB by using bidimensional interpolation; to do this we record data in a line buffer to have memory of the previous two lines. line buffer has 32 bits and its 16 lower bits contain pevious line while its 16 upper bits contain the second last line. With a total of 3 lines it's possible to interpolate using up to 5 pixels, depending on the kind of data. Interpolation being done is represented in the following table:

<table>
    <tr><th colspan=4>Green Pattern 1            </th> <th colspan=4>Green Pattern 2            </th> <th colspan=4>Red/Blue Pattern 1         </th> <th colspan=4>Red/Blue Pattern 2         </th> <th colspan=4>Red/Blue Pattern 3         </th> <th colspan=4>Red/Blue Pattern 4</th></tr>
    <tr><td>R/B</td><td> 1 </td><td>R/B</td> <th></th> <td> 1 </td><td>R/B</td><td> 1 </td> <th></th> <td> G </td><td> 1 </td><td> G </td> <th></th> <td> G </td><td>R/B</td><td> G </td> <th></th> <td>R/B</td><td> G </td><td>R/B</td> <th></th> <td> 1 </td><td> G </td><td> 1 </td></tr> 
    <tr><td> 1 </td><td>B/R</td><td> 1 </td> <th></th> <td>B/R</td><td> 4 </td><td>B/R</td> <th></th> <td>R/B</td><td> G </td><td>R/B</td> <th></th> <td> 1 </td><td> G </td><td> 1 </td> <th></th> <td> G </td><td> 1 </td><td> G </td> <th></th> <td> G </td><td>R/B</td><td> G </td></tr> 
    <tr><td>R/B</td><td> 1 </td><td>R/B</td> <th></th> <td> 1 </td><td>R/B</td><td> 1 </td> <th></th> <td> G </td><td> 1 </td><td> G </td> <th></th> <td> G </td><td>R/B</td><td> G </td> <th></th> <td>R/B</td><td> G </td><td>R/B</td> <th></th> <td> 1 </td><td> G </td><td> 1 </td></tr>
</table>

