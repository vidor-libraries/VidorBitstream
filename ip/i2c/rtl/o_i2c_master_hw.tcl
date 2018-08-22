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

# 
# o_i2c_master "I2C Master" v1.0
#  2018.03.29.11:04:13
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module o_i2c_master
# 
set_module_property DESCRIPTION ""
set_module_property NAME o_i2c_master
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP ipTronix
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "I2C Master"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset quartus_synth QUARTUS_SYNTH "" "Quartus Synthesis"
set_fileset_property quartus_synth TOP_LEVEL i2c_master_top
set_fileset_property quartus_synth ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property quartus_synth ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file i2c_master_bit_ctrl.v VERILOG PATH i2c_master_bit_ctrl.v
add_fileset_file i2c_master_byte_ctrl.v VERILOG PATH i2c_master_byte_ctrl.v
add_fileset_file i2c_master_defines.v VERILOG PATH i2c_master_defines.v
add_fileset_file i2c_master_top.v VERILOG PATH i2c_master_top.v
add_fileset_file timescale.v VERILOG PATH timescale.v

add_fileset sim_verilog SIM_VERILOG "" "Verilog Simulation"
set_fileset_property sim_verilog TOP_LEVEL o_i2c_master
set_fileset_property sim_verilog ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property sim_verilog ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file i2c_master_bit_ctrl.v VERILOG PATH i2c_master_bit_ctrl.v
add_fileset_file i2c_master_byte_ctrl.v VERILOG PATH i2c_master_byte_ctrl.v
add_fileset_file i2c_master_defines.v VERILOG PATH i2c_master_defines.v
add_fileset_file i2c_master_top.v VERILOG PATH i2c_master_top.v
add_fileset_file timescale.v VERILOG PATH timescale.v


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clock_sink
# 
add_interface clock_sink clock end
set_interface_property clock_sink clockRate 0
set_interface_property clock_sink ENABLED true
set_interface_property clock_sink EXPORT_OF ""
set_interface_property clock_sink PORT_NAME_MAP ""
set_interface_property clock_sink CMSIS_SVD_VARIABLES ""
set_interface_property clock_sink SVD_ADDRESS_GROUP ""

add_interface_port clock_sink wb_clk_i clk Input 1


# 
# connection point clock_sink_reset
# 
add_interface clock_sink_reset reset end
set_interface_property clock_sink_reset associatedClock clock_sink
set_interface_property clock_sink_reset synchronousEdges DEASSERT
set_interface_property clock_sink_reset ENABLED true
set_interface_property clock_sink_reset EXPORT_OF ""
set_interface_property clock_sink_reset PORT_NAME_MAP ""
set_interface_property clock_sink_reset CMSIS_SVD_VARIABLES ""
set_interface_property clock_sink_reset SVD_ADDRESS_GROUP ""

add_interface_port clock_sink_reset wb_rst_i reset Input 1

# 
# connection point clock_sink_areset
# 
add_interface clock_sink_areset reset end
#set_interface_property clock_sink_areset associatedClock clock_sink
set_interface_property clock_sink_areset synchronousEdges NONE
set_interface_property clock_sink_areset ENABLED true
set_interface_property clock_sink_areset EXPORT_OF ""
set_interface_property clock_sink_areset PORT_NAME_MAP ""
set_interface_property clock_sink_areset CMSIS_SVD_VARIABLES ""
set_interface_property clock_sink_areset SVD_ADDRESS_GROUP ""
add_interface_port clock_sink_areset arst_i reset_n Input 1


# 
# connection point avalon_slave_0
# 
add_interface avalon_slave_0 avalon end
set_interface_property avalon_slave_0 addressUnits WORDS
set_interface_property avalon_slave_0 associatedClock clock_sink
set_interface_property avalon_slave_0 associatedReset clock_sink_reset
set_interface_property avalon_slave_0 bitsPerSymbol 8
set_interface_property avalon_slave_0 burstOnBurstBoundariesOnly false
set_interface_property avalon_slave_0 burstcountUnits WORDS
set_interface_property avalon_slave_0 explicitAddressSpan 0
set_interface_property avalon_slave_0 holdTime 0
set_interface_property avalon_slave_0 linewrapBursts false
set_interface_property avalon_slave_0 maximumPendingReadTransactions 0
set_interface_property avalon_slave_0 maximumPendingWriteTransactions 0
set_interface_property avalon_slave_0 readLatency 1
set_interface_property avalon_slave_0 readWaitTime 0
set_interface_property avalon_slave_0 setupTime 0
set_interface_property avalon_slave_0 timingUnits Cycles
set_interface_property avalon_slave_0 writeWaitTime 0
set_interface_property avalon_slave_0 ENABLED true
set_interface_property avalon_slave_0 EXPORT_OF ""
set_interface_property avalon_slave_0 PORT_NAME_MAP ""
set_interface_property avalon_slave_0 CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave_0 SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave_0 wb_ack_o waitrequest_n Output 1
add_interface_port avalon_slave_0 wb_adr_i address Input 3
add_interface_port avalon_slave_0 wb_dat_i writedata Input 32
add_interface_port avalon_slave_0 wb_stb_i chipselect Input 1
add_interface_port avalon_slave_0 wb_we_i write Input 1
add_interface_port avalon_slave_0 wb_dat_o readdata Output 32
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point conduit_start
# 
add_interface conduit_start conduit end
set_interface_property conduit_start associatedClock ""
set_interface_property conduit_start associatedReset ""
set_interface_property conduit_start ENABLED true
set_interface_property conduit_start EXPORT_OF ""
set_interface_property conduit_start PORT_NAME_MAP ""
set_interface_property conduit_start CMSIS_SVD_VARIABLES ""
set_interface_property conduit_start SVD_ADDRESS_GROUP ""

add_interface_port conduit_start scl_pad_i scl_i Input  1
add_interface_port conduit_start scl_pad_o scl_o Output 1
add_interface_port conduit_start scl_padoen_o scl_en Output 1

add_interface_port conduit_start sda_pad_i sda_i Input 1
add_interface_port conduit_start sda_pad_o sda_o Output 1
add_interface_port conduit_start sda_padoen_o sda_en Output 1


# 
# connection point interrupt_sender
# 
add_interface interrupt_sender interrupt end
set_interface_property interrupt_sender associatedAddressablePoint ""
set_interface_property interrupt_sender associatedClock clock_sink
set_interface_property interrupt_sender bridgedReceiverOffset ""
set_interface_property interrupt_sender bridgesToReceiver ""
set_interface_property interrupt_sender ENABLED true
set_interface_property interrupt_sender EXPORT_OF ""
set_interface_property interrupt_sender PORT_NAME_MAP ""
set_interface_property interrupt_sender CMSIS_SVD_VARIABLES ""
set_interface_property interrupt_sender SVD_ADDRESS_GROUP ""

add_interface_port interrupt_sender wb_inta_o irq Output 1

