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


#
# altera_generic_quad_spi_controller_sw.tcl
#

# Create a new driver
create_driver iptronix_generic_quad_spi_controller2

# Associate it with some hardware known as "iptronix_generic_quad_spi_controller2"
set_sw_property hw_class_name iptronix_generic_quad_spi_controller2

# The version of this driver
set_sw_property version 18.0

# This driver may be incompatible with versions of hardware less
# than specified below. Updates to hardware and device drivers
# rendering the driver incompatible with older versions of
# hardware are noted with this property assignment.
set_sw_property min_compatible_hw_version 18.0

# Initialize the driver in alt_sys_init()
set_sw_property auto_initialize true

# This driver only works when the following combinations of interfaces 
# are enabled and connected as a group of CSR interfaces.
set_sw_property csr_interfaces "avl_mem,avl_csr"

# Interrupt properties: This driver supports enhanced
# interrupt APIs, as well as ISR preemption.
set_sw_property isr_preemption_supported true
set_sw_property supported_interrupt_apis "enhanced_interrupt_api"

# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory drivers

#
# Source file listings...
#

# Header files
add_sw_property include_source HAL/inc/iptronix_generic_quad_spi_controller2.h
add_sw_property include_source inc/iptronix_generic_quad_spi_controller2_regs.h

# C/C++ source files
add_sw_property c_source HAL/src/iptronix_generic_quad_spi_controller2.c

# This driver supports HAL & UCOSII BSP (OS) types
add_sw_property supported_bsp_type HAL 
add_sw_property supported_bsp_type UCOSII

# End of file