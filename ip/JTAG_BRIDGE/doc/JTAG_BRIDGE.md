# **JTAG_BRIDGE** IP core

Implements a bridge between a virtual JTAG interface and an avalon bus.

## Ports

<table>
    <tr><th          >Signal Group  </th><th>Signal Name     </th> <th>Direction</th> <th>Width</th> <th>Description                   </th> </tr>
    <tr><th rowspan=2>Clock/Reset   </th><td>iCLK            </td> <td>Input    </td> <td>  1  </td> <td>Avalon clock                  </td> </tr>
    <tr>                                 <td>iRESET          </td> <td>Input    </td> <td>  1  </td> <td>Avalon Reset                  </td> </tr>
    <tr><th rowspan=8>Avalon Master </th><td>oADDRESS        </td> <td>Output   </td> <td> 32  </td> <td>Address                       </td> </tr>
    <tr>                                 <td>oWRITE          </td> <td>Output   </td> <td>  1  </td> <td>Write request                 </td> </tr>
    <tr>                                 <td>oREAD           </td> <td>Output   </td> <td>  1  </td> <td>Read request                  </td> </tr>
    <tr>                                 <td>oWRITE_DATA     </td> <td>Output   </td> <td> 32  </td> <td>Fata to be written            </td> </tr>
    <tr>                                 <td>iREAD_DATA      </td> <td>Input    </td> <td> 32  </td> <td>Fata being read               </td> </tr>
    <tr>                                 <td>oBURST_COUNT    </td> <td>Output   </td> <td>  5  </td> <td>Number of words to read/write </td> </tr>
    <tr>                                 <td>iWAIT_REQUEST   </td> <td>Input    </td> <td>  1  </td> <td>Wait request from slave       </td> </tr>
    <tr>                                 <td>iREAD_DATA_VALID</td> <td>Input    </td> <td>  1  </td> <td>data valid flag from slave    </td> </tr>
</table>

## Parameters

The IP has no parameters

## Description

**JTAG_BRIDGE** core allows translating Virtual JTAG commands into avalon bus transactions. Virtual JTAG is an Intel IP core that implements a virtual JTAG scan chain that allows having multiple IP blocks each with a virtual instruction register (VIR) and a virtual data register (VDR).
Bridge operates in a similar way to I2C in the sense that it has an instruction for setting address and writing data and an instruction to read data using the address previously set by the write instruction (eventually with no payload). As wit I2C it is possible to read and write multiple words in a single transaction, in which case address will automatically increment.
VIR size is 3 bits and implements the following commands:

<table>
    <tr><th>Command           </th> <th>VIR value</th> <th>Description                                                              </th> </tr>
    <tr><td>Write/Set address </td> <td>   0     </td> <td>sets address and, if data is present, writes data to slave               </td> </tr>
    <tr><td>Read              </td> <td>   1     </td> <td>Reads data from address previously set through Write instruction         </td> </tr>
</table>

To perform a write transaction it's necessary to set VIR to 0 and then write in VDR address and data. 
In order to optimize JTAG transactions address can be set up with 8, 16, 24 or 32 bits. since data bus is 32 bit, the two LSBs of the address are used to determine address size. This means that if the two LSBs of the address are 00 the IP expects only 8 address bits after which subsequent bits are treated as data words to write. If a read operation is required, after address bits are sent transaction can be concluded.

**Note** that read operation reads data before data is shifted out so this means that first word is read as soon as VIR is set to 1 and a new word is read as soon as the previous one starts being shifted out. This means that an extra read will be performed when shifting out the last bit of the last word being read out from jtag and care should be taken if that register has bits that change value if read as those values would be lost.
