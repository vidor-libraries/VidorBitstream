# **NEOPIXEL** IP core

Drives multiple neopixel strings of LEDs

## Ports

<table>
    <tr><th          >Signal Group  </th><th>Signal Name          </th> <th>Direction</th> <th>Width</th> <th>Description                   </th> </tr>
    <tr><th rowspan=2>Clock/Reset   </th><td>iCLOCK               </td> <td>Input    </td> <td>  1  </td> <td>Clock input                   </td> </tr>
    <tr>                                 <td>iRESET               </td> <td>Input    </td> <td>  1  </td> <td>Reset input                   </td> </tr>
    <tr><th rowspan=6>Avalon Slave  </th><td>iCSR_ADDRESS         </td> <td>Input    </td> <td> 32  </td> <td>Address                       </td> </tr>
    <tr>                                 <td>iCSR_WRITE           </td> <td>Input    </td> <td>  1  </td> <td>Write request                 </td> </tr>
    <tr>                                 <td>iCSR_READ            </td> <td>Input    </td> <td>  1  </td> <td>Read request                  </td> </tr>
    <tr>                                 <td>iCSR_WRITE_DATA      </td> <td>Input    </td> <td> 32  </td> <td>Fata to be written            </td> </tr>
    <tr>                                 <td>oCSR_READ_DATA       </td> <td>Output   </td> <td> 32  </td> <td>Fata being read               </td> </tr>
    <tr>                                 <td>oCSR_READ_DATA_VALID </td> <td>Output   </td> <td>  1  </td> <td>data valid flag from slave    </td> </tr>
    <tr><th rowspan=6>Avalon Master </th><td>oDATA_ADDRESS        </td> <td>Output   </td> <td> 32  </td> <td>Address                       </td> </tr>
    <tr>                                 <td>oDATA_READ           </td> <td>Output   </td> <td>  1  </td> <td>Read request                  </td> </tr>
    <tr>                                 <td>iDATA_READ_DATA      </td> <td>Input    </td> <td> 32  </td> <td>Fata being read               </td> </tr>
    <tr>                                 <td>oDATA_BURST_COUNT    </td> <td>Output   </td> <td>  5  </td> <td>Number of words to read/write </td> </tr>
    <tr>                                 <td>iDATA_WAIT_REQUEST   </td> <td>Input    </td> <td>  1  </td> <td>Wait request from slave       </td> </tr>
    <tr>                                 <td>iDATA_READ_DATA_VALID</td> <td>Input    </td> <td>  1  </td> <td>data valid flag from slave    </td> </tr>
</table>

## Parameters

<table>
    <tr><th>Parameter     </th> <th>Allowed range          </th> <th>Description                                      </th> </tr>   
    <tr><td>pCHANNELS     </td> <td>    any                </td> <td>Number of LED strings to drive                   </td> </tr>   
    <tr><td>pSTART_ADDRESS</td> <td>0x00000000 : 0xFFFFFFFC</td> <td>Memory start address of the neopixel data buffer </td> </tr>   
</table>

## Description

**NEOPIXEL** core allows driving neopixel strings via hardware. Once set up the core autonomously reads from memory and serializes data to multiple channels sharing the same setup. This basically means that a single ip core can drive more than one string of LEDs but all of them have to have the same frequency, timing and memory buffer set up.
Since Avalon master interface supports wait request and burst it can be connected both to on chip RAM and to external DRAM controllers. **Note** that in order to work properly the IP block should receive data to shift out before it is actually required. 
