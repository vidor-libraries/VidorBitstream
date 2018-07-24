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
# altera_16550_uart_driver.tcl
#

# Create a new driver
create_driver iptronix_16550_uart_driver

# Associate it with some hardware known as "altera_16550_uart"
set_sw_property hw_class_name iptronix_16550_uart

# The version of this driver
set_sw_property version 18.0

# This driver may be incompatible with versions of hardware less
# than specified below. Updates to hardware and device drivers
# rendering the driver incompatible with older versions of
# hardware are noted with this property assignment.
#
# Multiple-Version compatibility was introduced in version 13.1;
# prior versions are therefore excluded.
set_sw_property min_compatible_hw_version 13.1

# Initialize the driver in alt_sys_init()
set_sw_property auto_initialize true

# Location in generated BSP that above sources will be copied into
set_sw_property bsp_subdirectory drivers

# Interrupt properties: This driver supports both legacy and enhanced
# interrupt APIs, as well as ISR preemption.
set_sw_property isr_preemption_supported true
set_sw_property supported_interrupt_apis "legacy_interrupt_api enhanced_interrupt_api"

#
# Source file listings...
#

# C/C++ source files
add_sw_property c_source HAL/src/iptronix_16550_uart_fd.c
add_sw_property c_source HAL/src/iptronix_16550_uart_init.c
add_sw_property c_source HAL/src/iptronix_16550_uart_ioctl.c
add_sw_property c_source HAL/src/iptronix_16550_uart_read.c
add_sw_property c_source HAL/src/iptronix_16550_uart_write.c


# Include files
add_sw_property include_source HAL/inc/iptronix_16550_uart.h
add_sw_property include_source HAL/inc/iptronix_16550_uart_fd.h
add_sw_property include_source inc/iptronix_16550_uart_regs.h


# This driver supports HAL & UCOSII BSP (OS) types
add_sw_property supported_bsp_type HAL
add_sw_property supported_bsp_type UCOSII

# Add per-driver configuration option for optional IOCTL functionality in
# UART driver.
add_sw_setting boolean_define_only public_mk_define enable_ioctl ALTERA_16550_UART_USE_IOCTL false "Enable driver ioctl() support. This feature is not compatible with the 'small' driver; ioctl() support will not be compiled if either the UART 'enable_small_driver' or HAL 'enable_reduced_device_drivers' settings are enabled."

# End of file
