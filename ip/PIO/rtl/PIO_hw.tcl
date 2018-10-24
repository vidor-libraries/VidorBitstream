# Copyright 2018 ARDUINO SA (http://www.arduino.cc/)
# This file is part of Vidor IP.
# Copyright (c) 2018
# Authors: Dario Pennisi
#
# This software is released under:
# The GNU General Public License, which covers the main part of 
# Vidor IP
# The terms of this license can be found at:
# https://www.gnu.org/licenses/gpl-3.0.en.html
#
# You can be released from the requirements of the above licenses by purchasing
# a commercial license. Buying such a license is mandatory if you want to modify or
# otherwise use the software for commercial activities involving the Arduino
# software without disclosing the source code of your own applications. To purchase
# a commercial license, send an email to license@arduino.cc.

package require -exact qsys 16.1


# 
# module PIO
# 
set_module_property DESCRIPTION ""
set_module_property NAME PIO
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP ipTronix
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME PIO
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false
set_module_property ELABORATION_CALLBACK            elaboration_callback


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL PIO
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file PIO.sv SYSTEM_VERILOG PATH PIO.sv TOP_LEVEL_FILE


# 
# parameters
# 
add_parameter pBITS INTEGER 32
set_parameter_property pBITS DEFAULT_VALUE 32
set_parameter_property pBITS DISPLAY_NAME Bits
set_parameter_property pBITS TYPE INTEGER
set_parameter_property pBITS UNITS None
set_parameter_property pBITS HDL_PARAMETER true
set_parameter_property pBITS ALLOWED_RANGES {1:32}
add_parameter pMUX_BITS INTEGER 2
set_parameter_property pMUX_BITS DEFAULT_VALUE 2
set_parameter_property pMUX_BITS DISPLAY_NAME "Mux Bits"
set_parameter_property pMUX_BITS TYPE INTEGER
set_parameter_property pMUX_BITS UNITS None
set_parameter_property pMUX_BITS HDL_PARAMETER true
set_parameter_property pMUX_BITS ALLOWED_RANGES {1:4}


# 
# display items
# 


# 
# connection point s1
# 
add_interface s1 avalon end
set_interface_property s1 addressUnits WORDS
set_interface_property s1 associatedClock clk
set_interface_property s1 associatedReset reset
set_interface_property s1 bitsPerSymbol 8
set_interface_property s1 burstOnBurstBoundariesOnly false
set_interface_property s1 burstcountUnits WORDS
set_interface_property s1 explicitAddressSpan 0
set_interface_property s1 holdTime 0
set_interface_property s1 linewrapBursts false
set_interface_property s1 maximumPendingReadTransactions 0
set_interface_property s1 maximumPendingWriteTransactions 0
set_interface_property s1 readLatency 1
set_interface_property s1 readWaitStates 0
set_interface_property s1 readWaitTime 0
set_interface_property s1 setupTime 0
set_interface_property s1 timingUnits Cycles
set_interface_property s1 writeWaitTime 0
set_interface_property s1 ENABLED true
set_interface_property s1 EXPORT_OF ""
set_interface_property s1 PORT_NAME_MAP ""
set_interface_property s1 CMSIS_SVD_VARIABLES ""
set_interface_property s1 SVD_ADDRESS_GROUP ""

add_interface_port s1 iWRITE write Input 1
add_interface_port s1 iREAD read Input 1
add_interface_port s1 iWRITE_DATA writedata Input 32
add_interface_port s1 oREAD_DATA readdata Output 32
set_interface_assignment s1 embeddedsw.configuration.isFlash 0
set_interface_assignment s1 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment s1 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment s1 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point clk
# 
add_interface clk clock end
set_interface_property clk clockRate 0
set_interface_property clk ENABLED true
set_interface_property clk EXPORT_OF ""
set_interface_property clk PORT_NAME_MAP ""
set_interface_property clk CMSIS_SVD_VARIABLES ""
set_interface_property clk SVD_ADDRESS_GROUP ""

add_interface_port clk iCLOCK clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clk
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset iRESET reset Input 1


# 
# connection point pio
# 
add_interface pio conduit end
set_interface_property pio associatedClock clk
set_interface_property pio associatedReset ""
set_interface_property pio ENABLED true
set_interface_property pio EXPORT_OF ""
set_interface_property pio PORT_NAME_MAP ""
set_interface_property pio CMSIS_SVD_VARIABLES ""
set_interface_property pio SVD_ADDRESS_GROUP ""

add_interface_port pio iPIO in Input 32
add_interface_port pio oDIR dir Output 32
add_interface_port pio oPIO out Output 32

# -----------------------------------------------------------------------------
proc log2 {value} {
  set value [expr $value-1]
  for {set log2 0} {$value>0} {incr log2} {
     set value  [expr $value>>1]
  }
  return $log2;
}

# -----------------------------------------------------------------------------
# callbacks
# -----------------------------------------------------------------------------
proc elaboration_callback {} {
  set bits [get_parameter_value pBITS] 
  set mux_bits [get_parameter_value pMUX_BITS] 
  add_interface_port s1 iADDRESS address Input [log2 [expr 4+ ( 31 + $bits * $mux_bits )/32 ]]
  add_interface_port pio oMUXSEL msel Output [expr  $bits * $mux_bits ]
}
