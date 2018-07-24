# (C) 2001-2018 Intel Corporation. All rights reserved.
# Your use of Intel Corporation's design tools, logic functions and other 
# software and tools, and its AMPP partner logic functions, and any output 
# files from any of the foregoing (including device programming or simulation 
# files), and any associated documentation or information are expressly subject 
# to the terms and conditions of the Intel Program License Subscription 
# Agreement, Intel FPGA IP License Agreement, or other applicable 
# license agreement, including, without limitation, that your use is for the 
# sole purpose of programming logic devices manufactured by Intel and sold by 
# Intel or its authorized distributors.  Please refer to the applicable 
# agreement for further details.


package require -exact qsys 12.0

# ------------------------------------------------------------------------------
# Module altera_16550_uart
# ------------------------------------------------------------------------------
set_module_property NAME                           iptronix_16550_uart
set_module_property VERSION                        18.0
set_module_property DISPLAY_NAME                   "16550 Compatible UART Intel FPGA IP"
set_module_property DESCRIPTION                    "UART with run-time configurability and 16550 compatible register space"
set_module_property GROUP                          "Interface Protocols/Serial"
set_module_property ELABORATION_CALLBACK           elaboration
set_module_property VALIDATION_CALLBACK            validation
set_module_property AUTHOR                         "Intel Corporation"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE   true
set_module_property EDITABLE                       true
set_module_property ANALYZE_HDL                    false
set_module_property ALLOW_GREYBOX_GENERATION       true
set_module_property HIDE_FROM_QUARTUS true
set_module_property DATASHEET_URL http://www.altera.com


# ------------------------------------------------------------------------------
# Parameters
# ------------------------------------------------------------------------------

# Master Parameters used by RTL

add_parameter           FAMILY         string          ""
set_parameter_property  FAMILY         SYSTEM_INFO     {DEVICE_FAMILY}
set_parameter_property  FAMILY         HDL_PARAMETER   true
set_parameter_property  FAMILY	       VISIBLE	       false

add_parameter           MEM_BLOCK_TYPE string          "AUTO"
set_parameter_property  MEM_BLOCK_TYPE DISPLAY_NAME    {Memory Block type}
set_parameter_property  MEM_BLOCK_TYPE DESCRIPTION     "Memory block used in RX path and TX path when FIFO mode is on"
set_parameter_property  MEM_BLOCK_TYPE ALLOWED_RANGES  {AUTO MRAM M4K M512 M9K M144K M20K M10K MLAB MRAM}
set_parameter_property  MEM_BLOCK_TYPE HDL_PARAMETER   true
set_parameter_property  MEM_BLOCK_TYPE VISIBLE	       true

add_parameter           FIFO_MODE      INTEGER 1
set_parameter_property  FIFO_MODE      DISPLAY_NAME "FIFO"
set_parameter_property  FIFO_MODE      DESCRIPTION "When enabled, FIFO buffer is used for transmit and receive data storage.<br/>When disabled, a single entry buffer is used for transmit and receive data storage."
set_parameter_property  FIFO_MODE      HDL_PARAMETER true
set_parameter_property  FIFO_MODE      ALLOWED_RANGES {"0: Disabled" "1: Enabled"}

add_parameter           FIFO_DEPTH     INTEGER 128
set_parameter_property  FIFO_DEPTH     HDL_PARAMETER true
set_parameter_property  FIFO_DEPTH     DISPLAY_NAME "Depth of FIFO"
set_parameter_property  FIFO_DEPTH     DESCRIPTION "Configures the depth of the FIFO."
set_parameter_property  FIFO_DEPTH     ALLOWED_RANGES {"32" "64" "128" "256"}

add_parameter           FIFO_WATERMARK INTEGER 0
set_parameter_property  FIFO_WATERMARK DISPLAY_NAME "FIFO Watermark"
set_parameter_property  FIFO_WATERMARK HDL_PARAMETER true
set_parameter_property  FIFO_WATERMARK ALLOWED_RANGES {"0: Disabled" "1: Enabled"}
#set_parameter_property  FIFO_WATERMARK STATUS "EXPERIMENTAL"
set_parameter_property  FIFO_WATERMARK VISIBLE false

add_parameter           FIFO_HWFC      INTEGER 1
set_parameter_property  FIFO_HWFC      DISPLAY_NAME "Hardware Flow Control"
set_parameter_property  FIFO_HWFC      DESCRIPTION "Enables Hardware Flow Control."
set_parameter_property  FIFO_HWFC      HDL_PARAMETER true
set_parameter_property  FIFO_HWFC      ALLOWED_RANGES {"0: Disabled" "1: Enabled"}

add_parameter           FIFO_SWFC      INTEGER 0
set_parameter_property  FIFO_SWFC      DISPLAY_NAME "Software Flow Control"
set_parameter_property  FIFO_SWFC      HDL_PARAMETER true
set_parameter_property  FIFO_SWFC      ALLOWED_RANGES {"0: Disabled" "1: Enabled"}
#set_parameter_property  FIFO_SWFC      STATUS "EXPERIMENTAL"
set_parameter_property  FIFO_SWFC      VISIBLE false

add_parameter           DMA_EXTRA      INTEGER 0
set_parameter_property  DMA_EXTRA      DISPLAY_NAME "Additional DMA Interface"
set_parameter_property  DMA_EXTRA      DESCRIPTION "Enables DMA interface.<br/>DMA interface support is limited to DMA Peripheral Request interface from Hard Processor System core only."
set_parameter_property  DMA_EXTRA      HDL_PARAMETER true
set_parameter_property  DMA_EXTRA      ALLOWED_RANGES {"0: Disabled" "1: Enabled"}

# SYSTEM_INFO parameter to obtain clockrate
add_parameter           clockRate      INTEGER 0
set_parameter_property  clockRate      DISPLAY_NAME {clockRate}
set_parameter_property  clockRate      VISIBLE {0}
set_parameter_property  clockRate      HDL_PARAMETER {0}
set_parameter_property  clockRate      SYSTEM_INFO {CLOCK_RATE clock}
set_parameter_property  clockRate      SYSTEM_INFO_TYPE {CLOCK_RATE}
set_parameter_property  clockRate      SYSTEM_INFO_ARG {clock}

add_parameter 			deviceFeatures STRING
set_parameter_property 	deviceFeatures DEFAULT_VALUE {NONE}
set_parameter_property 	deviceFeatures DISPLAY_NAME {deviceFeatures}
set_parameter_property 	deviceFeatures VISIBLE {0}
set_parameter_property 	deviceFeatures AFFECTS_GENERATION {1}
set_parameter_property 	deviceFeatures HDL_PARAMETER {0}
set_parameter_property 	deviceFeatures SYSTEM_INFO {device_feature}
set_parameter_property 	deviceFeatures SYSTEM_INFO_TYPE {DEVICE_FEATURES}

#-------------------------------------------------------------------------------
# module validation
#-------------------------------------------------------------------------------
proc proc_updated_legal_blocks_type { deviceFeatures } {
  set legal_blocks [list]
  lappend legal_blocks "AUTO"
  array set device_feature_array $deviceFeatures   
  
  foreach mem_feature [array names device_feature_array] {
      if { [ string match "*_MEMORY*" "$mem_feature" ] } {                                               
          if { $device_feature_array($mem_feature) } {	    
      	      switch $mem_feature {
                  "MRAM_MEMORY"  { lappend legal_blocks "MRAM" }
                  "M4K_MEMORY"   { lappend legal_blocks "M4K" }
                  "M512_MEMORY"  { lappend legal_blocks "M512" }
                  "M9K_MEMORY"   { lappend legal_blocks "M9K" }
                  "M144K_MEMORY" { lappend legal_blocks "M144K" }
                  "M20K_MEMORY"  { lappend legal_blocks "M20K" }
                  "M10K_MEMORY"  { lappend legal_blocks "M10K" }
                  "MLAB_MEMORY"  { lappend legal_blocks "MLAB" }
                  default { send_message error "$mem_feature is not a valid ram type" }
              } 
          }      
      }
  }                       
  set_parameter_property MEM_BLOCK_TYPE ALLOWED_RANGES "$legal_blocks"
}

# ------------------------------------------------------------------------------
# Validation Callback
# ------------------------------------------------------------------------------
proc validation {} {

   #What do I wanna check here?
   #1. FIFO Depth is 2^N, N>=1
   #2. FIFO Watermark dependency on FIFO_MODE
   #3. FIFO_HWFC dependency on FIFO_MODE
   #4. FIFO_SWFC dependency on FIFO_MODE
   
   #Get various parameter values we care about
   set fifo_mode      [get_parameter_value FIFO_MODE]
   set fifo_depth     [get_parameter_value FIFO_DEPTH]
   set fifo_h2om      [get_parameter_value FIFO_WATERMARK]
   set fifo_hwfc      [get_parameter_value FIFO_HWFC]
   set fifo_swfc      [get_parameter_value FIFO_SWFC]
   set deviceFeatures [get_parameter_value deviceFeatures]

   proc_updated_legal_blocks_type $deviceFeatures
   
   if {$fifo_mode == 0} {
      if {$fifo_h2om != 0} {  send_message Error "FIFO Watermark can only be enabled when FIFO is enabled"  }
      if {$fifo_hwfc != 0} {  send_message Error "Hardware Flow Control can only be enabled when FIFO is enabled"  }
      if {$fifo_swfc != 0} {  send_message Error "Software Flow Control can only be enabled when FIFO is enabled"  }
	  
	  set_parameter_property  MEM_BLOCK_TYPE ENABLED	       false
   } else {
	  set_parameter_property  MEM_BLOCK_TYPE ENABLED	       true
   }

   #Set various assignments to be used by Software
   set_module_assignment embeddedsw.dts.vendor "altr"
   set_module_assignment embeddedsw.dts.group "serial"
   set_module_assignment embeddedsw.dts.params.reg-shift 2
   set_module_assignment embeddedsw.dts.params.reg-io-width 4
   
   #Set clock frequency
   set clockRate [ get_parameter_value clockRate ]
   set_module_assignment  embeddedsw.CMacro.FREQ  $clockRate
   set_module_assignment  embeddedsw.CMacro.FIFO_MODE  $fifo_mode
   set_module_assignment  embeddedsw.CMacro.FIFO_DEPTH  $fifo_depth
   set_module_assignment  embeddedsw.CMacro.FIO_HWFC  $fifo_hwfc
   set_module_assignment  embeddedsw.CMacro.FIO_SWFC  $fifo_swfc
   set_module_assignment  embeddedsw.dts.params.clock-frequency  $clockRate

   send_message Info "Clock Rate is set at $clockRate"

   #Set FIFO mode and depth
   if {$fifo_mode == 1} {
      #FIFO enabled at generate time
      set_module_assignment embeddedsw.dts.compatible "altr,16550-FIFO$fifo_depth ns16550a"
      set_module_assignment embeddedsw.dts.params.fifo-size $fifo_depth
   } else {
      #FIFO disabled at generate time
      set_module_assignment embeddedsw.dts.compatible "ns8250"
      set_module_assignment embeddedsw.dts.params.fifo-size 1
   }
   
   # Set hardware flow control
   if {$fifo_hwfc == 1} {
      set_module_assignment {embeddedsw.dts.params.auto-flow-control} 1
   }
    
}



# ------------------------------------------------------------------------------
# Elaboration Callback
# ------------------------------------------------------------------------------
proc elaboration {} {

    set dma_extra     [get_parameter_value DMA_EXTRA]

    if { $dma_extra == 0 } {
        set_port_property   dma_tx_req_n     termination true
        set_port_property   dma_rx_req_n     termination true
        set_port_property   dma_tx_single_n  termination true
        set_port_property   dma_rx_single_n  termination true
        set_port_property   dma_tx_ack_n     termination true
        set_port_property   dma_rx_ack_n     termination true
        set_port_property   dma_tx_ack_n     termination_value 1
        set_port_property   dma_rx_ack_n     termination_value 1 
    }

}


# ------------------------------------------------------------------------------
# Interface
# ------------------------------------------------------------------------------
#
# Avalon-MM Slave
#
add_interface           avalon_slave avalon slave
add_interface_port      avalon_slave addr       address     Input    9
add_interface_port      avalon_slave write      write       Input    1
add_interface_port      avalon_slave writedata  writedata   Input    32
add_interface_port      avalon_slave read       read        Input    1
add_interface_port      avalon_slave readdata   readdata    Output   32
set_interface_property  avalon_slave associatedClock                 clock
set_interface_property  avalon_slave associatedReset                 reset_sink
set_interface_property  avalon_slave addressUnits                    SYMBOLS
set_interface_property  avalon_slave bitsPerSymbol                   8
set_interface_property  avalon_slave maximumPendingReadTransactions  0
set_interface_property  avalon_slave readLatency                     1
set_interface_property  avalon_slave readWaitTime                    0
set_interface_property  avalon_slave writeWaitTime                   0
set_interface_property  avalon_slave timingUnits                     Cycles
set_interface_property  avalon_slave ENABLED                         true

# 
# Clock Sink
# 
add_interface           clock       clock       sink
add_interface_port      clock       clk         clk         Input    1
set_interface_property  clock       ENABLED     true

# 
# Reset Sink
# 
add_interface           reset_sink  reset       sink
add_interface_port      reset_sink  rst_n       reset_n     Input    1
set_interface_property  reset_sink  associatedClock         clock
set_interface_property  reset_sink  synchronousEdges        DEASSERT
set_interface_property  reset_sink  ENABLED     true

# 
# Interrupt Source
# 
add_interface           irq_sender  interrupt   sender
add_interface_port      irq_sender  intr        irq         Output   1
set_interface_property  irq_sender  associatedClock         clock
set_interface_property  irq_sender  associatedReset         reset_sink
set_interface_property  irq_sender  associatedAddressablePoint       avalon_slave
set_interface_property  irq_sender  ENABLED     true

# 
# connection point RS_232_Serial
# 
add_interface           RS_232_Serial  conduit  end
add_interface_port      RS_232_Serial  sin      sin         Input    1
add_interface_port      RS_232_Serial  sout     sout        Output   1
add_interface_port      RS_232_Serial  sout_oe  sout_oe     Output   1
set_interface_property  RS_232_Serial  ENABLED  true

# 
# connection point RS_232_Modem
# 
add_interface           RS_232_Modem conduit end
add_interface_port      RS_232_Modem cts_n      cts_n       Input    1
add_interface_port      RS_232_Modem rts_n      rts_n       Output   1
add_interface_port      RS_232_Modem dsr_n      dsr_n       Input    1
add_interface_port      RS_232_Modem dcd_n      dcd_n       Input    1
add_interface_port      RS_232_Modem ri_n       ri_n        Input    1
add_interface_port      RS_232_Modem dtr_n      dtr_n       Output   1
add_interface_port      RS_232_Modem out1_n     out1_n      Output   1
add_interface_port      RS_232_Modem out2_n     out2_n      Output   1
set_interface_property  RS_232_Modem ENABLED    true


# 
# connection point DMA_Handshaking_tx
# 
add_interface DMA_Handshaking_tx conduit end
set_interface_property  DMA_Handshaking_tx associatedClock ""
set_interface_property  DMA_Handshaking_tx associatedReset ""
set_interface_property  DMA_Handshaking_tx ENABLED true
set_interface_property  DMA_Handshaking_tx EXPORT_OF ""
set_interface_property  DMA_Handshaking_tx PORT_NAME_MAP ""
set_interface_property  DMA_Handshaking_tx SVD_ADDRESS_GROUP ""
add_interface_port      DMA_Handshaking_tx dma_tx_ack_n dma_ack Input 1
add_interface_port      DMA_Handshaking_tx dma_tx_req_n dma_req Output 1
add_interface_port      DMA_Handshaking_tx dma_tx_single_n dma_single Output 1

# 
# connection point DMA_Handshaking_rx
# 
add_interface DMA_Handshaking_rx conduit end
set_interface_property  DMA_Handshaking_rx associatedClock ""
set_interface_property  DMA_Handshaking_rx associatedReset ""
set_interface_property  DMA_Handshaking_rx ENABLED true
set_interface_property  DMA_Handshaking_rx EXPORT_OF ""
set_interface_property  DMA_Handshaking_rx PORT_NAME_MAP ""
set_interface_property  DMA_Handshaking_rx SVD_ADDRESS_GROUP ""
add_interface_port      DMA_Handshaking_rx dma_rx_ack_n dma_ack Input 1
add_interface_port      DMA_Handshaking_rx dma_rx_req_n dma_req Output 1
add_interface_port      DMA_Handshaking_rx dma_rx_single_n dma_single Output 1


# ------------------------------------------------------------------------------
# File List
# ------------------------------------------------------------------------------
# 
# file sets
#
add_fileset             synthesis_fileset       QUARTUS_SYNTH       proc_add_synth_encrypt_file 
set_fileset_property    synthesis_fileset       TOP_LEVEL           altera_16550_uart 

add_fileset             sim_verilog_fileset     SIM_VERILOG         proc_add_sim_encrypt_file
set_fileset_property    sim_verilog_fileset     TOP_LEVEL           altera_16550_uart 

add_fileset             sim_vhdl_fileset        SIM_VHDL            proc_add_sim_encrypt_file
set_fileset_property    sim_vhdl_fileset        TOP_LEVEL           altera_16550_uart 


proc proc_add_synth_encrypt_file { entity_name } {
    add_fileset_file        altr_uart_csr.v            VERILOG        PATH  "csr/altr_uart_csr.v"
    add_fileset_file        altr_uart_bit_timer.sv     SYSTEM_VERILOG PATH  "altr_uart_bit_timer.sv"
    add_fileset_file        altr_uart_clkgen.sv        SYSTEM_VERILOG PATH  "altr_uart_clkgen.sv"
    add_fileset_file        altr_uart_csrext.sv        SYSTEM_VERILOG PATH  "altr_uart_csrext.sv"
    add_fileset_file        altr_uart_data_cnt.sv      SYSTEM_VERILOG PATH  "altr_uart_data_cnt.sv"
    add_fileset_file        altr_uart_databuffer.sv    SYSTEM_VERILOG PATH  "altr_uart_databuffer.sv"
    add_fileset_file        altr_uart_fifo.sv          SYSTEM_VERILOG PATH  "altr_uart_fifo.sv"
    add_fileset_file        altr_uart_fifomem.sv       SYSTEM_VERILOG PATH  "altr_uart_fifomem.sv"
    add_fileset_file        altr_uart_interrupt.sv     SYSTEM_VERILOG PATH  "altr_uart_interrupt.sv"
    add_fileset_file        altr_uart_modem.sv         SYSTEM_VERILOG PATH  "altr_uart_modem.sv"
    add_fileset_file        altr_uart_rx_shift.sv      SYSTEM_VERILOG PATH  "altr_uart_rx_shift.sv"
    add_fileset_file        altr_uart_rx_sync.sv       SYSTEM_VERILOG PATH  "altr_uart_rx_sync.sv"
    add_fileset_file        altr_uart_rxfc.sv          SYSTEM_VERILOG PATH  "altr_uart_rxfc.sv"
    add_fileset_file        altr_uart_rxfsm.sv         SYSTEM_VERILOG PATH  "altr_uart_rxfsm.sv"
    add_fileset_file        altr_uart_rxin.sv          SYSTEM_VERILOG PATH  "altr_uart_rxin.sv"
    add_fileset_file        altr_uart_rxstor.sv        SYSTEM_VERILOG PATH  "altr_uart_rxstor.sv"
    add_fileset_file        altr_uart_rxswfc.sv        SYSTEM_VERILOG PATH  "altr_uart_rxswfc.sv"
    add_fileset_file        altr_uart_sync.sv          SYSTEM_VERILOG PATH  "altr_uart_sync.sv"
    add_fileset_file        altr_uart_tx_shift.sv      SYSTEM_VERILOG PATH  "altr_uart_tx_shift.sv"
    add_fileset_file        altr_uart_txfc.sv          SYSTEM_VERILOG PATH  "altr_uart_txfc.sv"
    add_fileset_file        altr_uart_txfsm.sv         SYSTEM_VERILOG PATH  "altr_uart_txfsm.sv"
    add_fileset_file        altr_uart_txout.sv         SYSTEM_VERILOG PATH  "altr_uart_txout.sv"
    add_fileset_file        altr_uart_txstor.sv        SYSTEM_VERILOG PATH  "altr_uart_txstor.sv"
    add_fileset_file        altera_16550_uart.sv       SYSTEM_VERILOG PATH  "top/altera_16550_uart.sv"
    add_fileset_file        altr_uart_dmaintf.sv       SYSTEM_VERILOG PATH  "altr_uart_dmaintf.sv"
    add_fileset_file        altera_16550_uart.ocp      OTHER          PATH  "top/altera_16550_uart.ocp"
}

proc proc_add_sim_encrypt_file { entity_name } {
   if {1} {
      add_vendor_encrypt_fileset_file mentor
   }
   if {1} {
      add_vendor_encrypt_fileset_file aldec
   }
   if {1} {
      add_vendor_encrypt_fileset_file cadence
   }
   if {1} {
      add_vendor_encrypt_fileset_file synopsys
   }    
}

proc add_vendor_encrypt_fileset_file { vendor } {
    set vendor_uppercase "[ string toupper $vendor ]"
    
    add_fileset_file        ${vendor}/altr_uart_csr.v            VERILOG_ENCRYPT        PATH  "${vendor}/csr/altr_uart_csr.v"          "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_bit_timer.sv     SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_bit_timer.sv"       "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_clkgen.sv        SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_clkgen.sv"          "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_csrext.sv        SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_csrext.sv"          "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_data_cnt.sv      SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_data_cnt.sv"        "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_databuffer.sv    SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_databuffer.sv"      "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_fifo.sv          SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_fifo.sv"            "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_fifomem.sv       SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_fifomem.sv"         "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_interrupt.sv     SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_interrupt.sv"       "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_modem.sv         SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_modem.sv"           "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_rx_shift.sv  SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_rx_shift.sv"    "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_rx_sync.sv       SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_rx_sync.sv"         "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_rxfc.sv          SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_rxfc.sv"            "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_rxfsm.sv         SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_rxfsm.sv"           "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_rxin.sv          SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_rxin.sv"            "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_rxstor.sv        SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_rxstor.sv"          "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_rxswfc.sv        SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_rxswfc.sv"          "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_sync.sv          SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_sync.sv"            "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_tx_shift.sv  SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_tx_shift.sv"    "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_txfc.sv          SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_txfc.sv"            "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_txfsm.sv         SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_txfsm.sv"           "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_txout.sv         SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_txout.sv"           "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_txstor.sv        SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_txstor.sv"          "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altera_16550_uart.sv       SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/top/altera_16550_uart.sv"     "${vendor_uppercase}_SPECIFIC"
    add_fileset_file        ${vendor}/altr_uart_dmaintf.sv       SYSTEM_VERILOG_ENCRYPT PATH  "${vendor}/altr_uart_dmaintf.sv"         "${vendor_uppercase}_SPECIFIC"
}

## Add documentation links for user guide and/or release notes
add_documentation_link "User Guide" https://documentation.altera.com/#/link/sfo1400787952932/iga1404147076304
add_documentation_link "Release Notes" https://documentation.altera.com/#/link/hco1416836145555/hco1416836653221
