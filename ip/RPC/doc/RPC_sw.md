# **RPC** Software IP core - Soft core software documentation

## High level description
The purpose of this software library is to implement high level communication between an external processor and a soft core running on the FPGA. Instantiation of the RPC module automatically instantiates a **MAILBOX** IP core which is used for the lower layer of communication.
**RPC** APIs add on top of the **MAILBOX** IP core by adding a method to discover IP blocks instantiated in the FPGA, their properties and allows calling functions defined inside drivers running on the soft core. Finally this core allows sending events to the external processor to flag internal core status or virtual interrupts.

## Remote Procedure Call
RPC is achieved by raising the *message available* pin from the external controller after writing procedure parameters in the mailbox. Procedure parameters are written at the start address of the mailbox and each parameter consists in a 32 bit word, where the first is used to identify the IP block instance, its eventual subchannel and the number of procedure to call. First Parameter is defined as follows:

<table>
<tr><th colspan=3> Parameter 0 - Procedure ID</th></tr>
<tr><th>31:24</th><th>23:12</th><th>11:0</th></tr>
<tr><td>Global Instance ID</td><td>Channel</td><td>Procedure ID</td><tr>
</table>

<a name="GIID"></a>**Global Instance ID** is the unique number assigned to each IP block instance at compile time. this means that two instances of the same IP Block will still have different GIIDs. GIID=0 is reserved for the FPGA subsystem which handles discovery and auxiliary functions. 

**Channel** is an optional identifier of the sub channel the RPC should refer to. This is intended for peripherals which have multiple ports such as a timer with multiple PWM outputs.

**Procedure ID** is the number of the procedure that is going to be called.

Subsequent words consist in the parameters passed to the procedure. 
Parameters are always encapsulated in 32 bit words (WORD), which are the smallest addressable units on the **MAILBOX**, with the convention that data is little endian and that is always left aligned. 
Data types longer than 32 bits but with fixed size are encapsulated with multiple WORDs. Variable length data sizes such as strings are passed as a sequence of WORDS containing a prefix WORD that indicates the length, in bytes of the following data.

## Return Value
When function execution completes the soft core flags completion by zering the requested instruction at first location of the mailbox. In addition to this return value is written from second WORD with the same convention of parameter passing.

## Discovery
Implementation of the discovery mechanism uses the following assumptions:
* each IP block type has a UID (Unique IDentification) whose length is 20 bits, so two different IP blocks implementing the same function will have two different UIDs while two instances of the same block will have the same UID.
* the RPC API includes a set of calls with [GIID](#GIID)=0 that allow discovering the IP blocks. this discovery mechanism allows retrieving an array of pairs of [GIID](#GIID) and UID, thus allowing to know how many instances exist of all blocks and with which number of channels.
* the RPC API also requires that each IP exposes a function to request the lock for one or more IP Block channels. This function *allocates* the resource and verifies that the pins to which the IP is mapped are consistent with the FPGA image being used

## FPGA Subsystem ([GIID](#GIID)=0) APIS

| Procedure ID |API prototype                                            | Description                                                            |
|--------------|---------------------------------------------------------|------------------------------------------------------------------------|
|             1| [``` uint32_t FPGAVersion(void)  ```](#FPGAVersion)     | requests FPGA version                                                  |
|             2| [``` uint32_t[] IPDiscover(void) ```](#IPDiscover)      | Requests FPGA the array of descriptors for all the available IP blocks |

### <a name="FPGAVersion"></a>uint32_t FPGAVersion(void)

Requests FPGA version. the most significant byte represents an arbitrary image class and if set to 0xB0 identifies a boot image, capable of upgrading flash. The remaning 24 bits are arbitrarily defined but it is recommended to use a major/minor/build representation containing 8 bits each.

### <a name="IPDiscover"></a>uint32_t[] IPDiscover(void)

Requests the array of descriptors for all the available IP Blocks. API returns a variable size array of uint32_t structures defined as:

<table>
<tr><th colspan=2> discover return value (uint32_t[])</th></tr>
<tr><th>31:20</th><th>19:0</th></tr>
<tr><td>Channel number</td><td>UID</td><tr>
</table>

the [GIID](#GIID) of each IP represented by tables entry is implicit in its position in the table. 

## Mandatory APIs for IP blocks

All drivers should implement at least the three following APIs. Remember that when APIs are called, the Procedure ID contains along with GID and procedure number, also a channel number which can be used by the APIs to determine which channel the API is meant to be used with. APIs that operate globally on the IP are called Global Initializers (GI) and in order to recognize them easily the function name starts with a capital letter. 

| Procedure ID |API prototype                                     | Description                                                            |
|--------------|--------------------------------------------------|------------------------------------------------------------------------|
|             1| [``` uint32_t Version(void)   ```](#Version)     | requests IP block and driver version                                   |
|             2| [``` uint32_t Setup(...) ``` ](#Setup)           | Initializes IP block passing the global set up parameters              |
|             3| [``` uint32_t begin(...) ``` ](#begin)           | locks a channel of the IP initializing it with passed parameters       |
|             4| [``` uint32_t end(void) ```  ](#end)             | unlocks a channel of the IP releasing related pins                     |
|             5| [``` uint32_t query(void) ```](#query)           | unlocks a channel of the IP releasing related pins                     |

### <a name="Version"></a>uint32_t Version(void)

Returns the version of the IP block (upper 16 bits) and of the driver running in the soft core (lower 16 bits). Optionally this API can also return additional information in subsequent words to identify its parametrization or its capabilities.

### <a name="Setup"></a>uint32_t Setup(...)

Initializes the IP block with the given parameters. Parameters are dependant on the IP block driver. among parameters there could also be at least one that allows disabling the IP block completely. 
Although it is up to driver implementation to determine if this API can be called while channels are already being used (which means this will impact them) generally operation is allowed so care should be taken in using these APIs when using IP blocks with multiple channels.

### <a name="begin"></a>uint32_t begin(...)

Returns an error code (0 if success) for the request. parameters are dependant on the IP block driver and usually contain the pin number(s) to which the IP channel should be bound. 
As a result of this call the channel is initialized and pin muxing is set up to the requested function. if resource was already used or pin is not available the API call will fail.

### <a name="end"></a>uint32_t end(void) 

Releases a channel of the IP and uninitializes the related pins. Note that this does not disable or uninitialize the whole IP but only one of its channels.

### <a name="query"></a>uint32_t query(void) 

Returns the list of pins that can be used by the specified IP instance. The variable sized structure returned is defined as an array of WORDS defined as follows:

<table>
<tr><th colspan=3> Pin function (uint32_t)</th></tr>
<tr><th>31:24    </th><th>23:16         </th><th>15:0            </th></tr>
<tr><td>Group    </td><td>function index</td><td><a href="#pin">Pin number</a></td><tr>
</table>

For example a SPI peripheral would have 4 function indices (MISO, MOSI, SCK, CS). Group indicates a logic aggregate of function indices to indicate the preferred assignments of block of pins. for example you may have a two groups, one containing the full SPI interface on some pins and another containing only MISO, MOSI and SCK on some other pins.
Although grouping is only logical and there usually should not be any issue using pins from different groups for the same IP channel, you should consult the specific IP driver to understand if this is allowed or not.

## Pin numbering and multiplexing

In order to simplify pin numbering, FPGA pins are grouped in ports, with a maximum of 32 bits each.

Ports are grouped logically and are specific to each board however for a given class of boards at least the main connectors are standardized, so for example MKR boards have at least the MKR port which contains A and D pins.

IP block outputs to pins are always routed through multiplexers which also have a GPIO setting so that it is possible to use the pin as a GPIO or have multiple functions tied to the same pin. The only exception to this is for blocks that require special pins which can't be used as general purpose IOs or that have special IO requirements (eg. LVDS or connection to specific peripherals).

pins are identified by a 11 bit code where the 6 MSBs are the port number and the 5 LSBs are the port bit. since it's possible to pack a pin number in 16 bits, the remaining 5 MSBs can be used to identify the multiplex index. 

<table><a name="pin"></a>
<tr><th colspan=3> Pin number definition (uint16_t)    </th></tr>
<tr><th>15:11    </th><th>10:5       </th><th>4:0      </th></tr>
<tr><td>MUX index</td><td>Port number</td><td>Pin number</td><tr>
</table>

## Events

FPGA is able to send events to the external processor using the dedicated pin from FPGA to processor. Events are stored in a dedicated FIFO which is filled by the IP blocks with their identifier along with an event id. Whenever there is at least 1 event in the queue the soft core will raise the RPC completion steadily until the events are read in full.
Events are read by using a dedicated JTAG Bridge instruction that allows reading the FIFO. 

<table>
<tr><th colspan=3> Event ID                                </th></tr>
<tr><th>31:24             </th><th>23:12  </th><th>11:0    </th></tr>
<tr><td>Global Instance ID</td><td>Channel</td><td>Event ID</td><tr>
</table>

on the external processor side data should be read until pin does not get set back to zero.

