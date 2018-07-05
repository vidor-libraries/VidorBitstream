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


package require -exact qsys 16.1

# 
# module altera_trace_wrapper
# 
set_module_property DESCRIPTION "This component is a generic QSPI controller which allows user to access any QSPI devices"
set_module_property NAME iptronix_generic_quad_spi_controller2
set_module_property VERSION 18.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP "ipTronix"
set_module_property AUTHOR "Intel Corporation"
set_module_property DISPLAY_NAME "ipTronix Generic QUAD SPI controller II Intel FPGA IP"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property HIDE_FROM_QUARTUS false
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false
set_module_property VALIDATION_CALLBACK 	validation
set_module_property COMPOSITION_CALLBACK    compose

# 
# parameters
#
# +-----------------------------------
# | device family info
# +-----------------------------------

set device_list {"Arria 10" "Cyclone V" "Arria V GZ" "Arria V" "Stratix V" "Max 10 FPGA" "Cyclone 10 LP" "Cyclone 10 GX" "Stratix 10"}    

set all_supported_SPI_list {"EPCQ16:EPCQ16-Micron Compatible" "EPCQ32:EPCQ32-Micron Compatible" "EPCQ64:EPCQ64-Micron Compatible" "EPCQ128:EPCQ128-Micron Compatible" "EPCQ256:EPCQ256-Micron Compatible" "EPCQL256:EPCQL256-Micron Compatible" "EPCQ512:EPCQ512-Micron Compatible" "EPCQL512:EPCQL512-Micron Compatible" "EPCQL1024:EPCQL1024-Micron Compatible" \
							"N25Q16:N25Q016A13ESF40" "N25Q32:N25Q032A13ESF40" "N25Q64:N25Q064A13ESF40" "N25Q128:N25Q128A13ESF40" "N25Q256:N25Q256A13ESF40" "N25Q256-L:N25Q256A11E1240(low voltage)" "N25Q512:MT25QL512ABA" "N25Q512-L:N25Q512A11G1240(low voltage)" "N25Q1024-L:N25Q00AA11G1240(low voltage)" \
							"Micron512:N25Q512A83GSF40F"}
							
set_module_property SUPPORTED_DEVICE_FAMILIES    $device_list

# system info parameters

add_parameter 			DEVICE_FAMILY 	STRING
set_parameter_property 	DEVICE_FAMILY 	SYSTEM_INFO 	{DEVICE_FAMILY}
set_parameter_property 	DEVICE_FAMILY 	VISIBLE 		false
set_parameter_property  DEVICE_FAMILY 	HDL_PARAMETER true

add_parameter clkFreq LONG
set_parameter_property clkFreq DEFAULT_VALUE {0}
set_parameter_property clkFreq DISPLAY_NAME {clkFreq}
set_parameter_property clkFreq VISIBLE {0}
set_parameter_property clkFreq AFFECTS_GENERATION {1}
set_parameter_property clkFreq HDL_PARAMETER {0}
set_parameter_property clkFreq SYSTEM_INFO {clock_rate clock_sink}
set_parameter_property clkFreq SYSTEM_INFO_TYPE {CLOCK_RATE}
set_parameter_property clkFreq SYSTEM_INFO_ARG {clock_sink}

add_parameter CS_WIDTH INTEGER 1
set_parameter_property CS_WIDTH DEFAULT_VALUE 1
set_parameter_property CS_WIDTH DISPLAY_NAME CS_WIDTH
set_parameter_property CS_WIDTH DERIVED true
set_parameter_property CS_WIDTH TYPE INTEGER
set_parameter_property CS_WIDTH VISIBLE false
set_parameter_property CS_WIDTH UNITS None
set_parameter_property CS_WIDTH ALLOWED_RANGES {1, 3}
set_parameter_property CS_WIDTH HDL_PARAMETER true

add_parameter ADDR_WIDTH INTEGER 19
set_parameter_property ADDR_WIDTH DEFAULT_VALUE 19
set_parameter_property ADDR_WIDTH DISPLAY_NAME ADDR_WIDTH
set_parameter_property ADDR_WIDTH DERIVED true
set_parameter_property ADDR_WIDTH TYPE INTEGER
set_parameter_property ADDR_WIDTH VISIBLE false
set_parameter_property ADDR_WIDTH UNITS None
# 16M-19bit, 32M-20bit, 64M-21bit, 128M-22bit, 256M-23bit, 512M-24bit, 1024M-25bit, 2048M-26bit...
set_parameter_property ADDR_WIDTH ALLOWED_RANGES {19, 20, 21, 22, 23, 24, 25, 26, 27, 28}	
set_parameter_property ADDR_WIDTH HDL_PARAMETER true

add_parameter ASI_WIDTH INTEGER 1
set_parameter_property ASI_WIDTH DEFAULT_VALUE 1
set_parameter_property ASI_WIDTH DISPLAY_NAME ASI_WIDTH
set_parameter_property ASI_WIDTH DERIVED true
set_parameter_property ASI_WIDTH TYPE INTEGER
set_parameter_property ASI_WIDTH VISIBLE false
set_parameter_property ASI_WIDTH UNITS None
set_parameter_property ASI_WIDTH ALLOWED_RANGES {1, 4}
set_parameter_property ASI_WIDTH HDL_PARAMETER true

add_parameter ASMI_ADDR_WIDTH INTEGER 24
set_parameter_property ASMI_ADDR_WIDTH DEFAULT_VALUE 24
set_parameter_property ASMI_ADDR_WIDTH DISPLAY_NAME ASMI_ADDR_WIDTH
set_parameter_property ASMI_ADDR_WIDTH DERIVED true
set_parameter_property ASMI_ADDR_WIDTH TYPE INTEGER
set_parameter_property ASMI_ADDR_WIDTH VISIBLE false
set_parameter_property ASMI_ADDR_WIDTH UNITS None
set_parameter_property ASMI_ADDR_WIDTH ALLOWED_RANGES {24, 32}		
set_parameter_property ASMI_ADDR_WIDTH HDL_PARAMETER true

add_parameter ENABLE_4BYTE_ADDR	INTEGER "0"
set_parameter_property ENABLE_4BYTE_ADDR DISPLAY_NAME "Enable 4-byte addressing mode"
set_parameter_property ENABLE_4BYTE_ADDR DESCRIPTION "Check to enable 4-byte addressing mode for device larger than 128Mbyte"
set_parameter_property ENABLE_4BYTE_ADDR AFFECTS_GENERATION true
set_parameter_property ENABLE_4BYTE_ADDR VISIBLE false
set_parameter_property ENABLE_4BYTE_ADDR HDL_PARAMETER true
set_parameter_property ENABLE_4BYTE_ADDR DERIVED true

add_parameter DDASI	INTEGER "0"
set_parameter_property DDASI DISPLAY_NAME "Disable dedicated Active Serial interface"
set_parameter_property DDASI DESCRIPTION "Check to route ASMIBLOCK signals to top level of design"
set_parameter_property DDASI AFFECTS_GENERATION true
set_parameter_property DDASI VISIBLE false
set_parameter_property DDASI DERIVED true

# SPI device selection
add_parameter FLASH_TYPE STRING "N25Q16"
set_parameter_property FLASH_TYPE DISPLAY_NAME "Configuration device type"
set_parameter_property FLASH_TYPE ALLOWED_RANGES $all_supported_SPI_list
set_parameter_property FLASH_TYPE DESCRIPTION "Select targeted flash devices"
set_parameter_property FLASH_TYPE AFFECTS_GENERATION true
set_parameter_property FLASH_TYPE VISIBLE true
set_parameter_property FLASH_TYPE DERIVED false

add_parameter IO_MODE STRING "QUAD"
set_parameter_property IO_MODE DISPLAY_NAME "Choose I/O mode"
set_parameter_property IO_MODE DESCRIPTION "Select extended data width when Fast Read operation is enabled"
set_parameter_property IO_MODE HDL_PARAMETER true
set_parameter_property IO_MODE AFFECTS_GENERATION true
set_parameter_property IO_MODE VISIBLE true

add_parameter CHIP_SELS INTEGER "1"
set_parameter_property CHIP_SELS DISPLAY_NAME "Number of Chip Selects used"
set_parameter_property CHIP_SELS ALLOWED_RANGES {1 2 3}
set_parameter_property CHIP_SELS DESCRIPTION "Number of EPCQ(L) devices that are attached and need a CHIPSEL"
set_parameter_property CHIP_SELS HDL_PARAMETER true
set_parameter_property CHIP_SELS AFFECTS_GENERATION true

add_parameter ATOM STRING "UNIDIRECTIONAL"
set_parameter_property ATOM DISPLAY_NAME "export unidirectional interface"
set_parameter_property ATOM ALLOWED_RANGES {"BIDIRECTIONAL" "UNIDIRECTIONAL"}
set_parameter_property ATOM DESCRIPTION "selects wether to export unidirectional or bidirectional interface"
set_parameter_property ATOM HDL_PARAMETER true
set_parameter_property ATOM AFFECTS_GENERATION true

# +-----------------------------------

# add system info parameter
add_parameter           deviceFeaturesSystemInfo   STRING 			"None"
set_parameter_property  deviceFeaturesSystemInfo   system_info		"DEVICE_FEATURES"
set_parameter_property  deviceFeaturesSystemInfo   VISIBLE false


proc proc_get_derive_addr_width {flash_type} {
    switch $flash_type {
        "EPCQ16" - "N25Q16" {
            return 19 
        }
		"EPCQ32" - "N25Q32" {
            return 20
		}
        "EPCQ64" - "N25Q64" {
            return 21
        }
        "EPCQ128" - "N25Q128" {
            return 22
        }
        "EPCQ256" - "EPCQL256" - "N25Q256" - "N25Q256-L" - "MT25QU256" - "MT25QL256" {
            return 23
        }
        "EPCQ512" - "EPCQL512" - "N25Q512" - "N25Q512-L" - "Micron512" - "MT25QU512" - "MT25QL512" {
            return 24
        }
        "EPCQL1024" - "N25Q1024-L" - "MT25QU01G" {
            return 25
        }
        default {
            # Should never enter this function
            send_message error "$flash_type is not a valid flash type"
        }
    }
}


# This proc is called by elaboration proc to set embeddedsw C Macros assignments 
# used by downstream tools
proc set_cmacros {is_qspi flash_type} {
    if {$is_qspi eq "true"} {
        set_module_assignment embeddedsw.CMacro.IS_EPCS 0
    } else {
        set_module_assignment embeddedsw.CMacro.IS_EPCS 1
    }

    #string name of flash
    set_module_assignment embeddedsw.CMacro.FLASH_TYPE $flash_type

    #page size in bytes
    set_module_assignment embeddedsw.CMacro.PAGE_SIZE 256
    
    #sector and subsector size in bytes
    set_module_assignment embeddedsw.CMacro.SUBSECTOR_SIZE 4096
    set_module_assignment embeddedsw.CMacro.SECTOR_SIZE 65536

    #set number of sectors
    switch $flash_type {
        "N25Q16" - "EPCQ16" {
            set_module_assignment embeddedsw.CMacro.NUMBER_OF_SECTORS 32
        }
        "EPCQ32" - "N25Q32" {
            set_module_assignment embeddedsw.CMacro.NUMBER_OF_SECTORS 64
		}
        "N25Q64" - "EPCQ64" {
            set_module_assignment embeddedsw.CMacro.NUMBER_OF_SECTORS 128
        }
        "N25Q128" - "EPCQ128" {
            set_module_assignment embeddedsw.CMacro.NUMBER_OF_SECTORS 256
        }
        "EPCQ256" - "EPCQL256" - "N25Q256" - "N25Q256-L" - "MT25QU256" - "MT25QL256" {
            set_module_assignment embeddedsw.CMacro.NUMBER_OF_SECTORS 512
        }
        "EPCQ512" - "EPCQL512" - "N25Q512" - "N25Q512-L" - "Micron512" - "MT25QU512" - "MT25QL512" {
            set_module_assignment embeddedsw.CMacro.NUMBER_OF_SECTORS 1024
        }
        "EPCQL1024" - "N25Q1024-L" - "MT25QU01G" {
            set_module_assignment embeddedsw.CMacro.NUMBER_OF_SECTORS 2048
        }
        default {
            # Should never enter this function
            send_message error "$flash_type is not a valid flash type"
        }
    }
}

proc validation { } {
    # --- check ini for hidden devices --- #
    set get_spi_list        [get_parameter_property FLASH_TYPE   ALLOWED_RANGES]
    set enable_MT25Q        [get_quartus_ini pgm_allow_mt25q ENABLED]
    if {$enable_MT25Q == 1} {
        lappend get_spi_list    "MT25QL256"
        lappend get_spi_list    "MT25QL512"
        lappend get_spi_list    "MT25QU256"
        lappend get_spi_list    "MT25QU512"
        lappend get_spi_list    "MT25QU01G"
    }
    set_parameter_property  FLASH_TYPE   ALLOWED_RANGES      $get_spi_list       
}

proc compose {} {
	# add component
	add_instance clk altera_clock_bridge 18.0
    set_instance_parameter_value clk {EXPLICIT_CLOCK_RATE} {0.0}
    set_instance_parameter_value clk {NUM_CLOCK_OUTPUTS} {1}

    add_instance reset altera_reset_bridge 18.0
    set_instance_parameter_value reset {ACTIVE_LOW_RESET} {1}
    set_instance_parameter_value reset {SYNCHRONOUS_EDGES} {deassert}
    set_instance_parameter_value reset {NUM_RESET_OUTPUTS} {1}
    set_instance_parameter_value reset {USE_RESET_REQUEST} {0}

	add_instance	asmi2_inst_qspi_ctrl iptronix_asmi_parallel2 18.0
	add_instance	addr_adaption_1 altera_qspi_address_adaption 18.0

	#--------------------------------------------- settings that exist in all procedure ------------------------------------------------
	set EPCQ_SPI_list {"EPCQ16" "EPCQ32" "EPCQ64" "EPCQ128" "EPCQ256" "EPCQL256" "EPCQ512" "EPCQL512" "EPCQL1024"}
	
	# QSPI that supported for 4-byte addressing - en4b_addr
	set supported_4byte_addr 	{"EPCQ256" "EPCQ512" "EPCQL256" "EPCQL512" "EPCQL1024" "N25Q256" "N25Q256-L" "N25Q512" "N25Q512-L" "N25Q1024-L" "Micron512"  "MT25QL256" "MT25QL512" "MT25QU256" "MT25QU512" "MT25QU01"}
	
	set QSPI_list {"EPCQ16" "EPCQ32" "EPCQ64" "EPCQ128" "EPCQ256" "EPCQL256" "EPCQ512" "EPCQL512" "EPCQL1024" \
							"N25Q16" "N25Q32" "N25Q64" "N25Q128" "N25Q256" "N25Q256-L" "N25Q512" "N25Q512-L" "N25Q1024-L" "Micron512"  "MT25QL256" "MT25QL512" "MT25QU256" "MT25QU512" "MT25QU01"}
	# devices that supported QSPI - Quad/Dual data width, asmi_dataout, asmi_sdoin, asmi_dataoe
	set supported_QSPI_devices_list {"Arria 10" "Cyclone V" "Arria V GZ" "Arria V" "Stratix V" "MAX 10 FPGA" "Cyclone 10 LP" "Cyclone 10 GX" "Stratix 10"}
	
	# devices that supported simulation
	set supported_sim_devices_list {"Arria 10" "Cyclone V" "Arria V GZ" "Arria V" "Stratix V" "Cyclone 10 GX"}
	
	set stacked_die_device {"EPCQL512" "EPCQL1024" "N25Q512-L" "N25Q1024-L" "Micron512" "MT25QU01G"}
	#-----------------------------------------------------------------------------------------------------------------------------------
		
	set FLASH_TYPE 				[ get_parameter_value FLASH_TYPE ]
	set IO_MODE 				[ get_parameter_value IO_MODE ]
	set DEVICE_FAMILY 			[ get_parameter_value DEVICE_FAMILY ]
	set CS_WIDTH 				[ get_parameter_value CS_WIDTH ]
	set CHIP_SELS			    [ get_parameter_value CHIP_SELS]
	set temp_addr_width 		[ proc_get_derive_addr_width [ get_parameter_value FLASH_TYPE ] ]
	set clkFreq 				[ get_parameter_value clkFreq ]
	set is_4byte_addr_support	"false"
	set is_qspi					"false"
	set is_epcq_spi				"false"
	set ENABLE_BULK_ERASE 		"true"
	
	# check whether SPI device support 4-byte addressing
	foreach re_spi_1   $supported_4byte_addr {
		if {$re_spi_1 eq $FLASH_TYPE} {
			set is_4byte_addr_support	"true"
			break;
		 }
	 }
	 
	#if { $clkFreq > 25000000 } {
	#	send_message error "Input clock frequency for Quad SPI controller need to be lesser than 25Mhz to operate properly"
	#}
	 
	if {$is_4byte_addr_support eq "true"} {
		set_parameter_value 	ENABLE_4BYTE_ADDR "1"
		set_parameter_value		ASMI_ADDR_WIDTH 32
	} else {
		set_parameter_value 	ENABLE_4BYTE_ADDR "0"
		set_parameter_value		ASMI_ADDR_WIDTH 24
	}
	
	foreach epcq_list_0  $EPCQ_SPI_list {
		if {$epcq_list_0 eq $FLASH_TYPE} {
			set is_epcq_spi	"true"
			break;
		 }
	 }
	 
    if {$is_epcq_spi eq "true"} {
		set_parameter_value		DDASI 0
	} else {
		set_parameter_value		DDASI 1
	}
	
   if {$is_epcq_spi eq "true" && [check_device_family_equivalence $DEVICE_FAMILY "MAX 10 FPGA"]} {
		send_message error "EPCQ devices are not supported in MAX 10 FPGA"
	}

	# check whether devices supporting multiple flash - only for Arria 10
    if {$is_epcq_spi eq "false" || [check_device_family_equivalence $DEVICE_FAMILY "Arria10"] || [check_device_family_equivalence $DEVICE_FAMILY "Cyclone10GX"]} {
        set is_multi_flash_support	"true"
        if {$CHIP_SELS eq 3 } {set_parameter_value 	ADDR_WIDTH 		[ expr $temp_addr_width + 2]}
        if {$CHIP_SELS eq 2 } {set_parameter_value 	ADDR_WIDTH 		[ expr $temp_addr_width + 1]}
        if {$CHIP_SELS eq 1 } {set_parameter_value 	ADDR_WIDTH 		$temp_addr_width }
    } else {
        set is_multi_flash_support	"false"
        set_parameter_value 	ADDR_WIDTH 		$temp_addr_width
    }
		
	if {[check_device_family_equivalence $DEVICE_FAMILY "MAX 10 FPGA"]} {
		set_parameter_property IO_MODE ALLOWED_RANGES {"QUAD"}
    } elseif {[check_device_family_equivalence $DEVICE_FAMILY "Cyclone 10 LP"]} { 
        # If C10LP, if use choose EPCQ device then it only supports STANDARD, if Micron then can be either
        if {$is_epcq_spi eq "true"} {
            set_parameter_property IO_MODE ALLOWED_RANGES {"STANDARD"}
        } else { 
            set_parameter_property IO_MODE ALLOWED_RANGES {"STANDARD" "QUAD"}
        }
    } elseif {[check_device_family_equivalence $DEVICE_FAMILY "Stratix 10"]} { 
        # if S10, only support Micron device, which means use dedicated GPIO, no asmi hardblock
        if {$is_epcq_spi eq "true"} {
            send_message error "EPCQ devices are not supported in Stratix 10"
        } else { 
            set_parameter_property IO_MODE ALLOWED_RANGES {"STANDARD" "QUAD"}
        }
    } else {
		set_parameter_property IO_MODE ALLOWED_RANGES {"STANDARD" "QUAD"}
	}
	# Extra error message for Cyclone 10 LP
   if {[check_device_family_equivalence $DEVICE_FAMILY "Cyclone 10 LP"]} { 
       if {$IO_MODE eq "QUAD" && $is_epcq_spi eq "true"} {
           send_message error "EPCQ devices with QUAD mode is not supported in Cyclone 10 LP"
       }
   }
	#-------------------------------- Set addr_adaption_1 --------------------------------------------------
	
	set_instance_parameter_value 	addr_adaption_1 FLASH_TYPE 			$FLASH_TYPE
	set_instance_parameter_value	addr_adaption_1 DDASI [ get_parameter_value DDASI ]
	set_instance_parameter_value	addr_adaption_1 IO_MODE $IO_MODE
	set_instance_parameter_value	addr_adaption_1 ASI_WIDTH [ get_parameter_value ASI_WIDTH ]
	set_instance_parameter_value	addr_adaption_1 CS_WIDTH $CS_WIDTH
	set_instance_parameter_value	addr_adaption_1 CHIP_SELS $CHIP_SELS
	set_instance_parameter_value	addr_adaption_1 ASMI_ADDR_WIDTH [ get_parameter_value ASMI_ADDR_WIDTH ]
	set_instance_parameter_value	addr_adaption_1 ADDR_WIDTH [ get_parameter_value ADDR_WIDTH ]
	set_instance_parameter_value	addr_adaption_1 ENABLE_4BYTE_ADDR [ get_parameter_value ENABLE_4BYTE_ADDR ]

	# check whether is QSPI devices
	foreach re_spi_0   $QSPI_list {
		if {$re_spi_0 eq $FLASH_TYPE} { 
			set is_qspi		"true"
			break;
		 }
	 }
	 
	if {[check_device_family_equivalence $DEVICE_FAMILY $supported_QSPI_devices_list]} {
		set is_qspi_devices_list	"true"
	} else {
		set is_qspi_devices_list	"false"
	}
	
	if {[check_device_family_equivalence $DEVICE_FAMILY $supported_sim_devices_list]} {
		set is_sim_devices_list	"true"
	} else {
		set is_sim_devices_list	"false"
	}
	
	#-------------------------------Set asmi2_inst_qspi_ctrl -------------------------------------------------
	#if {[check_device_family_equivalence $DEVICE_FAMILY "MAX 10 FPGA"]} {
	#	set_instance_parameter_value 	asmi2_inst_qspi_ctrl DEVICE_FAMILY 		"Arria V"
	#} else {
	#	set_instance_parameter_value 	asmi2_inst_qspi_ctrl DEVICE_FAMILY 		$DEVICE_FAMILY
	#}
	
	foreach non_bulk_erase_device   $stacked_die_device {
		if {$non_bulk_erase_device eq $FLASH_TYPE} {
			set ENABLE_BULK_ERASE	"false"
			break;
		 }
	 }
	

	if { $is_multi_flash_support eq "true"} {
		set_parameter_value CS_WIDTH 3
		set_parameter_property	CHIP_SELS	ENABLED		true
	} else {
		set_parameter_value CS_WIDTH 1
		set_parameter_property	CHIP_SELS	ENABLED		false
	}
	
	# apply_instance_preset
	if { $FLASH_TYPE eq "N25Q16"} {		#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q016
	} elseif { $FLASH_TYPE eq "N25Q32"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q032
	} elseif { $FLASH_TYPE eq "N25Q64"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q064
	} elseif { $FLASH_TYPE eq "N25Q128"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q128
	} elseif { $FLASH_TYPE eq "N25Q256"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q256
	} elseif { $FLASH_TYPE eq "N25Q256-L"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q256
	} elseif { $FLASH_TYPE eq "N25Q512"} {	#Micron MT is single die preset
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			MT25QL512A
	} elseif { $FLASH_TYPE eq "N25Q512-L"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q512
	} elseif { $FLASH_TYPE eq "N25Q1024-L"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q00A
	} elseif {$FLASH_TYPE eq "EPCQ16"} {		#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q016
	} elseif {$FLASH_TYPE eq "EPCQ32"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q032
	} elseif {$FLASH_TYPE eq "EPCQ64"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q064
	} elseif {$FLASH_TYPE eq "EPCQ128"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q128
	} elseif {$FLASH_TYPE eq "EPCQ256"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q256
	} elseif {$FLASH_TYPE eq "EPCQL256"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q256
	} elseif {$FLASH_TYPE eq "EPCQ512"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			MT25QL512A
	} elseif {$FLASH_TYPE eq "Micron512"} {	#Micron dual dies
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q512A8 
	} elseif { $FLASH_TYPE eq "EPCQL512"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q512 
	} elseif { $FLASH_TYPE eq "EPCQL1024"} {	#Micron
		apply_instance_preset 	asmi2_inst_qspi_ctrl  			N25Q00A
	} else {								#Other preset should be same name
		apply_instance_preset   asmi2_inst_qspi_ctrl    ${FLASH_TYPE}
	}

	if {$is_qspi_devices_list eq "true" && $is_qspi eq "true"} {
		set_instance_parameter_value 	asmi2_inst_qspi_ctrl DATA_WIDTH 		$IO_MODE
		set_parameter_value ASI_WIDTH 4
   } else {
		set_parameter_value ASI_WIDTH 1
		set_instance_parameter_value 	asmi2_inst_qspi_ctrl DATA_WIDTH 		"STANDARD"
	}

	# check this again this ASMI2
	#if { $FLASH_TYPE eq "Micron512" } {
	#	set_instance_parameter_value 	asmi2_inst_qspi_ctrl FLASH_RSTPIN			true
	#} else {
	#	set_instance_parameter_value 	asmi2_inst_qspi_ctrl FLASH_RSTPIN			false
	#}
	

	if {$is_epcq_spi eq "true"} {
		set_instance_parameter_value 	asmi2_inst_qspi_ctrl gui_use_asmiblock		false
	} else {
		set_instance_parameter_value 	asmi2_inst_qspi_ctrl gui_use_asmiblock		true
	}
	
	if {$is_sim_devices_list eq "true"} {
		set_instance_parameter_value 	asmi2_inst_qspi_ctrl ENABLE_SIM_MODEL		true
	} else {
		set_instance_parameter_value 	asmi2_inst_qspi_ctrl ENABLE_SIM_MODEL		false
	}
	
	set_cmacros $is_qspi $FLASH_TYPE
	
	#------------------------ Export ports to flash ----------------------------------------------------------------
	# 
	# connection point conduit_end
	#
	if {[ get_parameter_value DDASI ] eq "1"} {
		set_instance_parameter_value 	asmi2_inst_qspi_ctrl CHIP_SELS	$CHIP_SELS
        if {[ get_parameter_value ATOM ] eq "UNIDIRECTIONAL"} {
			set_instance_parameter_value 	asmi2_inst_qspi_ctrl gui_use_gpio	false
			set_instance_parameter_value    asmi2_inst_qspi_ctrl gui_use_asmiblock true
			add_interface qspi_pins conduit end
			set_interface_property qspi_pins EXPORT_OF asmi2_inst_qspi_ctrl.atom_ports
			set_interface_property qspi_pins PORT_NAME_MAP "atom_ports_dclk qspi_pins_dclk atom_ports_ncs qspi_pins_ncs atom_ports_dataout qspi_pins_dataout atom_ports_datain qspi_pins_datain atom_ports_dataoe qspi_pins_dataoe atom_ports_oe qspi_pins_oe"
		} else {
			# turn on use PSI pin in asmi2_inst_qspi_ctrl
			set_instance_parameter_value 	asmi2_inst_qspi_ctrl gui_use_gpio	true
			add_interface qspi_pins conduit end
			set_interface_property qspi_pins EXPORT_OF asmi2_inst_qspi_ctrl.qspi_pins
			set_interface_property qspi_pins PORT_NAME_MAP "flash_dclk_out qspi_pins_dclk flash_ncs qspi_pins_ncs flash_dataout qspi_pins_data"
		}

		
		#set_interface_property qspi_pins PORT_NAME_MAP "flash_ncs qspi_pins_ncs"
		#set_interface_property qspi_pins PORT_NAME_MAP "flash_dataout qspi_pins_data"
		#add_interface flash_dataout conduit end
		#add_interface_port flash_dataout flash_dataout conduit_dataout Bidir 4
		
		#add_interface flash_dclk_out conduit end
		#add_interface_port flash_dclk_out flash_dclk_out conduit_dclk_out Output 1
		
		#add_interface flash_ncs conduit end
		#add_interface_port flash_ncs flash_ncs conduit_ncs Output CS_WIDTH
	} else {
        # If use EPCQ, only for A10 and C10, need to set the chip select to 3 
        # other devices, set to 1
        if {[check_device_family_equivalence $DEVICE_FAMILY "Arria10"] || [check_device_family_equivalence $DEVICE_FAMILY "Cyclone10GX"]} {
            set_instance_parameter_value 	asmi2_inst_qspi_ctrl gui_use_gpio	false
            set_instance_parameter_value 	asmi2_inst_qspi_ctrl CHIP_SELS	3
        } else {
            set_instance_parameter_value 	asmi2_inst_qspi_ctrl gui_use_gpio	false
            set_instance_parameter_value 	asmi2_inst_qspi_ctrl CHIP_SELS	1
        }
    }
	add_connection clk.out_clk reset.clk clock
    add_connection clk.out_clk asmi2_inst_qspi_ctrl.clk clock
    add_connection clk.out_clk addr_adaption_1.clock_sink clock
    add_connection reset.out_reset asmi2_inst_qspi_ctrl.reset reset
    add_connection reset.out_reset addr_adaption_1.reset reset

    add_connection addr_adaption_1.asmi_mem asmi2_inst_qspi_ctrl.avl_mem avalon
    add_connection addr_adaption_1.asmi_csr asmi2_inst_qspi_ctrl.avl_csr avalon
	add_connection addr_adaption_1.chip_select asmi2_inst_qspi_ctrl.chip_select conduit
    # exported interfaces
    add_interface avl_csr avalon slave
    set_interface_property avl_csr EXPORT_OF addr_adaption_1.avl_csr
    add_interface avl_mem avalon slave
    set_interface_property avl_mem EXPORT_OF addr_adaption_1.avl_mem
    add_interface interrupt_sender interrupt sender
    set_interface_property interrupt_sender EXPORT_OF addr_adaption_1.interrupt_sender
    add_interface clock_sink clock sink
    set_interface_property clock_sink EXPORT_OF clk.in_clk
    add_interface reset reset sink
    set_interface_property reset EXPORT_OF reset.in_reset
	
	rename_port_name
}

proc rename_port_name { } {
    #read interfaces from compose components
    foreach interface [get_interfaces] {
        #send_message INFO "interface found: $interface"
        # For each inteface, find the port in the child instance
        # and overwrite them to same name as child
        if {$interface eq "clock_sink"} {
            set_interface_property clock_sink PORT_NAME_MAP "clk in_clk"
        } elseif {$interface eq "reset"} {
            set_interface_property reset PORT_NAME_MAP "reset_n in_reset_n"
        } else {
        	# remap port name but not qspi_pins interface since this is in asmi2 component
        	if {$interface ne "qspi_pins" && $interface ne "atom_ports"} {
            	foreach port [get_instance_interface_ports addr_adaption_1 $interface] {
                	set the_ports($port) $port
            	}
            	set_interface_property $interface PORT_NAME_MAP [array get the_ports]
            }
        }
        
    }
}


# +-------------------------------------
# | Add settings needed by Nios tools
# +-------------------------------------
# Tells us component is a flash 
set_module_assignment embeddedsw.memoryInfo.IS_FLASH 1

# interface assignments for embedded software
#set_interface_assignment avl_mem embeddedsw.configuration.isFlash 1
#set_interface_assignment avl_mem embeddedsw.configuration.isMemoryDevice 1
#set_interface_assignment avl_mem embeddedsw.configuration.isNonVolatileStorage 1
#set_interface_assignment avl_mem embeddedsw.configuration.isPrintableDevice 0

# These assignments tells tools to create byte-addressed .hex files only
set_module_assignment embeddedsw.memoryInfo.GENERATE_HEX 1
set_module_assignment embeddedsw.memoryInfo.USE_BYTE_ADDRESSING_FOR_HEX 1
set_module_assignment embeddedsw.memoryInfo.GENERATE_DAT_SYM 0
set_module_assignment embeddedsw.memoryInfo.GENERATE_FLASH 0

# Width of memory
set_module_assignment embeddedsw.memoryInfo.MEM_INIT_DATA_WIDTH 32

# Output directories for programming files
#set_module_assignment embeddedsw.memoryInfo.DAT_SYM_INSTALL_DIR {SIM_DIR}
#set_module_assignment embeddedsw.memoryInfo.FLASH_INSTALL_DIR {APP_DIR}
set_module_assignment embeddedsw.memoryInfo.HEX_INSTALL_DIR {QPF_DIR}

# Module assignments related to names of simulation files
#set_module_assignment postgeneration.simulation.init_file.param_name {INIT_FILENAME}
#set_module_assignment postgeneration.simulation.init_file.type {MEM_INIT}

# +-------------------------------------
# | Add settings needed by DTG tools
# +-------------------------------------
# add device tree properties
set_module_assignment embeddedsw.dts.vendor "altr"
set_module_assignment embeddedsw.dts.name "quadspi"
set_module_assignment embeddedsw.dts.group "quadspi"
set_module_assignment embeddedsw.dts.compatible "altr,quadspi-2.0"
## Add documentation links for user guide and/or release notes
add_documentation_link "User Guide" https://documentation.altera.com/#/link/sfo1400787952932/iga1431459053861
add_documentation_link "Release Notes" https://documentation.altera.com/#/link/hco1421698042087/hco1421698013408
