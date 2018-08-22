# **FBST** IP core

CRT Controller that translates a streaming video input port into a displayable output stream

## Ports

<table>
    <tr><th          >Signal Group   </th><th>Signal Name   </th> <th>Direction</th> <th>Width</th> <th>Description                   </th> </tr>   
    <tr><th          >Clock/Reset    </th><td>iCLK          </td> <td>Output   </td> <td>  1  </td> <td>Pixel clock                   </td> </tr>   
    <tr><th rowspan=6>Video Port     </th><td>oRED          </td> <td>Output   </td> <td>  8  </td> <td>Red color output              </td> </tr>   
    <tr>                                  <td>oGRN          </td> <td>Output   </td> <td>  8  </td> <td>Green color output            </td> </tr>   
    <tr>                                  <td>oBLU          </td> <td>Output   </td> <td>  8  </td> <td>Blue color output             </td> </tr>   
    <tr>                                  <td>oHS           </td> <td>Output   </td> <td>  1  </td> <td>Horizontal Sync output        </td> </tr>   
    <tr>                                  <td>oVS           </td> <td>Output   </td> <td>  1  </td> <td>Vertical Sync output          </td> </tr>   
    <tr>                                  <td>oDE           </td> <td>Output   </td> <td>  1  </td> <td>Data Enable output            </td> </tr>   
    <tr><th rowspan=4>Streaming Input</th><td>iFB_START     </td> <td>Input    </td> <td>  1  </td> <td>Stream start flag             </td> </tr>   
    <tr>                                  <td>iFB_DATA      </td> <td>Input    </td> <td> 31  </td> <td>Stream data                   </td> </tr>
    <tr>                                  <td>iFB_DATA_VALID</td> <td>Input    </td> <td>  1  </td> <td>Stream data valid flag        </td> </tr>
    <tr>                                  <td>oFB_READY     </td> <td>Output   </td> <td>  1  </td> <td>Stream ready flag             </td> </tr>
</table>

## Parameters

<table>
    <tr><th          >Group         </th><th>Parameter   </th> <th>Allowed range</th> <th>Description                                 </th> </tr>   
    <tr><th rowspan=8>Video Timings </th><td>pHRES       </td> <td>    0-2043   </td> <td>Active video horizontal resolution          </td> </tr>   
    <tr>                                 <td>pVRES       </td> <td>    0-2043   </td> <td>Active video vertical resolution            </td> </tr>   
    <tr>                                 <td>pHTOTAL     </td> <td>    0-2043   </td> <td>Total number of clocks per line             </td> </tr>   
    <tr>                                 <td>pVTOTAL     </td> <td>    0-2043   </td> <td>Total number of lines per frame             </td> </tr>   
    <tr>                                 <td>pHSS        </td> <td>    0-2043   </td> <td>Horizontal Sync Start                       </td> </tr>   
    <tr>                                 <td>pHSE        </td> <td>    0-2043   </td> <td>Horizontal Sync End                         </td> </tr>   
    <tr>                                 <td>pVSS        </td> <td>    0-2043   </td> <td>Vertical Sync Start                         </td> </tr>   
    <tr>                                 <td>pVSE        </td> <td>    0-2043   </td> <td>Vertical Sync End                           </td> </tr>   
</table>

## Description

**FBST** core is a *CRT Controller* that inputs streaming video and outputs RGB data plus timing signals (HS/VS/DE). can be used in conjunction with DVI_OUT to produce video output directly connectable to a DVI/HDMI monitor.
The streaming video port accepts 31 bits formed as two 15 bit video ports and 1 video select flag. this allows blending two streams using a 1 bit alpha channel.
In order to reduce core size and simplify its structure it requires streaming data to be at pixel clock rate and has timings parametrizable at compilation time.
Video timings are specified by assuming active video window starts at coordinates (0,0). This means that horizontal sync must be specified to start after at least pHRES clocks and to end at least after pHSS and before pHTOTAL.

