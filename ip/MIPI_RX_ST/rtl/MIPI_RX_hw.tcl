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
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module MIPI_RX_ST
# 
set_module_property DESCRIPTION ""
set_module_property NAME MIPI_RX_ST
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP ipTronix
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME "MIPI CSI2 streaming Receiver"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL MIPI_RX
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file MIPI_RX_ST.v VERILOG PATH MIPI_RX_ST.v TOP_LEVEL_FILE


# 
# parameters
# 


# 
# display items
# 


# 
# connection point mipi
# 
add_interface mipi conduit end
set_interface_property mipi associatedClock ""
set_interface_property mipi associatedReset ""
set_interface_property mipi ENABLED true
set_interface_property mipi EXPORT_OF ""
set_interface_property mipi PORT_NAME_MAP ""
set_interface_property mipi CMSIS_SVD_VARIABLES ""
set_interface_property mipi SVD_ADDRESS_GROUP ""

add_interface_port mipi iMIPI_CLK clk Input 1
add_interface_port mipi iMIPI_D data Input 2


# 
# connection point mipi_st
# 
add_interface mipi_st conduit end
set_interface_property mipi_st associatedClock ""
set_interface_property mipi_st associatedReset ""
set_interface_property mipi_st ENABLED true
set_interface_property mipi_st EXPORT_OF ""
set_interface_property mipi_st PORT_NAME_MAP ""
set_interface_property mipi_st CMSIS_SVD_VARIABLES ""
set_interface_property mipi_st SVD_ADDRESS_GROUP ""

add_interface_port mipi_st oMIPI_DATA data Output 24
add_interface_port mipi_st oMIPI_START start Output 1
add_interface_port mipi_st oMIPI_DATA_VALID dv Output 1

