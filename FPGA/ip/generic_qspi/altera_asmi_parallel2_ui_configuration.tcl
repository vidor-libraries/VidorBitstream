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



add_display_item "Settings" "Common Operations"   GROUP
add_display_item "Settings" "Miscellaneous Operations"   GROUP
add_display_item "Settings" "Write Operations"   GROUP
add_display_item "Settings" "Read Operations"   GROUP
add_display_item "Common Operations" "Common_table" GROUP TABLE
add_display_item "Miscellaneous Operations" "Miscellaneous_table" GROUP TABLE
add_display_item "Write Operations" "Write_Table" GROUP TABLE
add_display_item "Write Operations"  POLL_OPCODE   parameter
add_display_item "Read Operations" "Read_Table" GROUP TABLE
set_display_item_property "Common_table" DISPLAY_HINT {table rows:10}
set_display_item_property "Miscellaneous_table" DISPLAY_HINT {table rows:13}
set_display_item_property "Write_Table" DISPLAY_HINT {table rows:4}
set_display_item_property "Read_Table" DISPLAY_HINT {table rows:4}


add_parameter WR_ID_LIST STRING 1
set_parameter_property WR_ID_LIST DISPLAY_NAME {WR_ID}
set_parameter_property WR_ID_LIST AFFECTS_ELABORATION true
set_parameter_property WR_ID_LIST AFFECTS_GENERATION true
set_parameter_property WR_ID_LIST HDL_PARAMETER false
set_parameter_property WR_ID_LIST GROUP "Configuration"
add_display_item        "Settings"   WR_ID_LIST           parameter

add_parameter WR_OPERATION_LIST STRING 1
set_parameter_property WR_OPERATION_LIST DISPLAY_NAME {WR_OPERATION_LIST}
set_parameter_property WR_OPERATION_LIST AFFECTS_ELABORATION true
set_parameter_property WR_OPERATION_LIST AFFECTS_GENERATION true
set_parameter_property WR_OPERATION_LIST HDL_PARAMETER false
add_display_item        "Settings"   WR_OPERATION_LIST           parameter

add_parameter WR_OPCODE_LIST STRING 1
set_parameter_property WR_OPCODE_LIST DISPLAY_NAME {WR_OPCODE_LIST}
set_parameter_property WR_OPCODE_LIST AFFECTS_ELABORATION true
set_parameter_property WR_OPCODE_LIST AFFECTS_GENERATION true
set_parameter_property WR_OPCODE_LIST HDL_PARAMETER false
add_display_item        "Settings"   WR_OPCODE_LIST           parameter

add_parameter RD_ID_LIST STRING 1
set_parameter_property RD_ID_LIST DISPLAY_NAME {RD_ID}
set_parameter_property RD_ID_LIST AFFECTS_ELABORATION true
set_parameter_property RD_ID_LIST AFFECTS_GENERATION true
set_parameter_property RD_ID_LIST HDL_PARAMETER false
set_parameter_property RD_ID_LIST GROUP "Configuration"
add_display_item        "Settings"   RD_ID_LIST           parameter

add_parameter RD_OPERATION_LIST STRING 1
set_parameter_property RD_OPERATION_LIST DISPLAY_NAME {RD_OPERATION_LIST}
set_parameter_property RD_OPERATION_LIST AFFECTS_ELABORATION true
set_parameter_property RD_OPERATION_LIST AFFECTS_GENERATION true
set_parameter_property RD_OPERATION_LIST HDL_PARAMETER false
add_display_item        "Settings"   RD_OPERATION_LIST           parameter

add_parameter RD_OPCODE_LIST STRING 1
set_parameter_property RD_OPCODE_LIST DISPLAY_NAME {RD_OPCODE_LIST}
set_parameter_property RD_OPCODE_LIST AFFECTS_ELABORATION true
set_parameter_property RD_OPCODE_LIST AFFECTS_GENERATION true
set_parameter_property RD_OPCODE_LIST HDL_PARAMETER false
add_display_item        "Settings"   RD_OPCODE_LIST           parameter


#----------------------------------------------------------------------
add_parameter COM_ID STRING_LIST 1
set_parameter_property COM_ID DISPLAY_NAME {No}
set_parameter_property COM_ID AFFECTS_ELABORATION true
set_parameter_property COM_ID AFFECTS_GENERATION true
set_parameter_property COM_ID HDL_PARAMETER false
set_parameter_property COM_ID DERIVED true
set_parameter_property COM_ID GROUP "Common_table"
set_parameter_property COM_ID DESCRIPTION {List of IDs}

add_parameter COM_OPERATION STRING_LIST "01"
set_parameter_property COM_OPERATION DISPLAY_NAME {Operation} 
set_parameter_property COM_OPERATION AFFECTS_ELABORATION true
set_parameter_property COM_OPERATION AFFECTS_GENERATION true
set_parameter_property COM_OPERATION HDL_PARAMETER false
set_parameter_property COM_OPERATION DERIVED true
set_parameter_property COM_OPERATION GROUP "Common_table"
set_parameter_property COM_OPERATION DESCRIPTION {List of channel IDs}

add_parameter COM_OPCODE STRING_LIST 0
set_parameter_property COM_OPCODE DISPLAY_NAME {Opcode}
set_parameter_property COM_OPCODE AFFECTS_ELABORATION true
set_parameter_property COM_OPCODE AFFECTS_GENERATION true
set_parameter_property COM_OPCODE HDL_PARAMETER false
set_parameter_property COM_OPCODE DERIVED false
set_parameter_property COM_OPCODE DEFAULT_VALUE {00 00 00 00 00 00}
set_parameter_property COM_OPCODE GROUP "Common_table"
set_parameter_property COM_OPCODE DISPLAY_HINT "hexadecimal"
set_parameter_property COM_OPCODE DESCRIPTION {List of opcode}

add_parameter COM_ADDR_BYTES STRING_LIST 0
set_parameter_property COM_ADDR_BYTES DISPLAY_NAME {Address bytes}
set_parameter_property COM_ADDR_BYTES AFFECTS_ELABORATION true
set_parameter_property COM_ADDR_BYTES AFFECTS_GENERATION true
set_parameter_property COM_ADDR_BYTES HDL_PARAMETER false
set_parameter_property COM_ADDR_BYTES DERIVED false
set_parameter_property COM_ADDR_BYTES DEFAULT_VALUE {0 0 0 0 0 0}
set_parameter_property COM_ADDR_BYTES GROUP "Common_table"
set_parameter_property COM_ADDR_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property COM_ADDR_BYTES DESCRIPTION {List of address bytes}

add_parameter COM_DATA_IN_BYTES STRING_LIST 0
set_parameter_property COM_DATA_IN_BYTES DISPLAY_NAME {Data in bytes}
set_parameter_property COM_DATA_IN_BYTES AFFECTS_ELABORATION true
set_parameter_property COM_DATA_IN_BYTES AFFECTS_GENERATION true
set_parameter_property COM_DATA_IN_BYTES HDL_PARAMETER false
set_parameter_property COM_DATA_IN_BYTES DERIVED false
set_parameter_property COM_DATA_IN_BYTES DEFAULT_VALUE {0 0 0 0 0 0}
set_parameter_property COM_DATA_IN_BYTES GROUP "Common_table"
set_parameter_property COM_DATA_IN_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property COM_DATA_IN_BYTES DESCRIPTION {List of data bytes}

add_parameter COM_DATA_OUT_BYTES STRING_LIST 0
set_parameter_property COM_DATA_OUT_BYTES DISPLAY_NAME {Data out bytes}
set_parameter_property COM_DATA_OUT_BYTES AFFECTS_ELABORATION true
set_parameter_property COM_DATA_OUT_BYTES AFFECTS_GENERATION true
set_parameter_property COM_DATA_OUT_BYTES HDL_PARAMETER false
set_parameter_property COM_DATA_OUT_BYTES DERIVED false
set_parameter_property COM_DATA_OUT_BYTES DEFAULT_VALUE {0 0 0 0 0 0}
set_parameter_property COM_DATA_OUT_BYTES GROUP "Common_table"
set_parameter_property COM_DATA_OUT_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property COM_DATA_OUT_BYTES DESCRIPTION {List of data bytes}


add_parameter COM_DUMMY_BYTES STRING_LIST 0
set_parameter_property COM_DUMMY_BYTES DISPLAY_NAME {Dummy bytes}
set_parameter_property COM_DUMMY_BYTES AFFECTS_ELABORATION true
set_parameter_property COM_DUMMY_BYTES AFFECTS_GENERATION true
set_parameter_property COM_DUMMY_BYTES HDL_PARAMETER false
set_parameter_property COM_DUMMY_BYTES DERIVED false
set_parameter_property COM_DUMMY_BYTES DEFAULT_VALUE {0 0 0 0 0 0}
set_parameter_property COM_DUMMY_BYTES GROUP "Common_table"
set_parameter_property COM_DUMMY_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property COM_DUMMY_BYTES DESCRIPTION {List of dummy bytes}

add_parameter MISC_EN STRING_LIST 0
set_parameter_property MISC_EN DISPLAY_NAME {Misc EN}
set_parameter_property MISC_EN AFFECTS_ELABORATION true
set_parameter_property MISC_EN AFFECTS_GENERATION true
set_parameter_property MISC_EN HDL_PARAMETER false
set_parameter_property MISC_EN DERIVED true
set_parameter_property MISC_EN DESCRIPTION {List of IDs}

add_parameter MISC_ID STRING_LIST 1
set_parameter_property MISC_ID DISPLAY_NAME {No}
set_parameter_property MISC_ID AFFECTS_ELABORATION true
set_parameter_property MISC_ID AFFECTS_GENERATION true
set_parameter_property MISC_ID HDL_PARAMETER false
set_parameter_property MISC_ID DERIVED true
set_parameter_property MISC_ID GROUP "Miscellaneous_table"
set_parameter_property MISC_ID DESCRIPTION {List of IDs}

add_parameter MISC_OPERATION STRING_LIST "01"
set_parameter_property MISC_OPERATION DISPLAY_NAME {Operation} 
set_parameter_property MISC_OPERATION AFFECTS_ELABORATION true
set_parameter_property MISC_OPERATION AFFECTS_GENERATION true
set_parameter_property MISC_OPERATION HDL_PARAMETER false
set_parameter_property MISC_OPERATION DERIVED true
set_parameter_property MISC_OPERATION GROUP "Miscellaneous_table"
set_parameter_property MISC_OPERATION DESCRIPTION {List of channel IDs}

add_parameter MISC_OPCODE STRING_LIST 0
set_parameter_property MISC_OPCODE DISPLAY_NAME {Opcode}
set_parameter_property MISC_OPCODE AFFECTS_ELABORATION true
set_parameter_property MISC_OPCODE AFFECTS_GENERATION true
set_parameter_property MISC_OPCODE HDL_PARAMETER false
set_parameter_property MISC_OPCODE DERIVED false
set_parameter_property MISC_OPCODE DEFAULT_VALUE {00 00 00 00 00 00 00 00 00 00}
set_parameter_property MISC_OPCODE GROUP "Miscellaneous_table"
set_parameter_property MISC_OPCODE DISPLAY_HINT "hexadecimal"
set_parameter_property MISC_OPCODE DESCRIPTION {List of opcode}


add_parameter MISC_ADDR_BYTES STRING_LIST 0
set_parameter_property MISC_ADDR_BYTES DISPLAY_NAME {Address bytes}
set_parameter_property MISC_ADDR_BYTES AFFECTS_ELABORATION true
set_parameter_property MISC_ADDR_BYTES AFFECTS_GENERATION true
set_parameter_property MISC_ADDR_BYTES HDL_PARAMETER false
set_parameter_property MISC_ADDR_BYTES DERIVED false
set_parameter_property MISC_ADDR_BYTES DEFAULT_VALUE {0 0 0 0 0 0 0 0 0 0}
set_parameter_property MISC_ADDR_BYTES GROUP "Miscellaneous_table"
set_parameter_property MISC_ADDR_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property MISC_ADDR_BYTES DESCRIPTION {List of address bytes}

add_parameter MISC_DATA_IN_BYTES STRING_LIST 0
set_parameter_property MISC_DATA_IN_BYTES DISPLAY_NAME {Data in bytes}
set_parameter_property MISC_DATA_IN_BYTES AFFECTS_ELABORATION true
set_parameter_property MISC_DATA_IN_BYTES AFFECTS_GENERATION true
set_parameter_property MISC_DATA_IN_BYTES HDL_PARAMETER false
set_parameter_property MISC_DATA_IN_BYTES DERIVED false
set_parameter_property MISC_DATA_IN_BYTES DEFAULT_VALUE {0 0 0 0 0 0 0 0 0 0}
set_parameter_property MISC_DATA_IN_BYTES GROUP "Miscellaneous_table"
set_parameter_property MISC_DATA_IN_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property MISC_DATA_IN_BYTES DESCRIPTION {List of data bytes}

add_parameter MISC_DATA_OUT_BYTES STRING_LIST 0
set_parameter_property MISC_DATA_OUT_BYTES DISPLAY_NAME {Data out bytes}
set_parameter_property MISC_DATA_OUT_BYTES AFFECTS_ELABORATION true
set_parameter_property MISC_DATA_OUT_BYTES AFFECTS_GENERATION true
set_parameter_property MISC_DATA_OUT_BYTES HDL_PARAMETER false
set_parameter_property MISC_DATA_OUT_BYTES DERIVED false
set_parameter_property MISC_DATA_OUT_BYTES DEFAULT_VALUE {0 0 0 0 0 0 0 0 0 0}
set_parameter_property MISC_DATA_OUT_BYTES GROUP "Miscellaneous_table"
set_parameter_property MISC_DATA_OUT_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property MISC_DATA_OUT_BYTES DESCRIPTION {List of data bytes}

add_parameter MISC_DUMMY_BYTES STRING_LIST 0
set_parameter_property MISC_DUMMY_BYTES DISPLAY_NAME {Dummy bytes}
set_parameter_property MISC_DUMMY_BYTES AFFECTS_ELABORATION true
set_parameter_property MISC_DUMMY_BYTES AFFECTS_GENERATION true
set_parameter_property MISC_DUMMY_BYTES HDL_PARAMETER false
set_parameter_property MISC_DUMMY_BYTES DERIVED false
set_parameter_property MISC_DUMMY_BYTES DEFAULT_VALUE {0 0 0 0 0 0 0 0 0 0}
set_parameter_property MISC_DUMMY_BYTES GROUP "Miscellaneous_table"
set_parameter_property MISC_DUMMY_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property MISC_DUMMY_BYTES DESCRIPTION {List of dummy bytes}

#----------------------------------------------------------------------

#----------------------------------------------------------------------
add_parameter WR_ID STRING_LIST 1
set_parameter_property WR_ID DISPLAY_NAME {No}
set_parameter_property WR_ID AFFECTS_ELABORATION true
set_parameter_property WR_ID AFFECTS_GENERATION true
set_parameter_property WR_ID HDL_PARAMETER false
set_parameter_property WR_ID DERIVED true
#set_parameter_property WR_ID GROUP "Write_Table"
set_parameter_property WR_ID DESCRIPTION {List of IDs}

add_parameter WR_OPERATION STRING_LIST "01"
set_parameter_property WR_OPERATION DISPLAY_NAME {Operation} 
set_parameter_property WR_OPERATION AFFECTS_ELABORATION true
set_parameter_property WR_OPERATION AFFECTS_GENERATION true
set_parameter_property WR_OPERATION HDL_PARAMETER false
set_parameter_property WR_OPERATION DERIVED true
#set_parameter_property WR_OPERATION GROUP "Write_Table"
set_parameter_property WR_OPERATION DESCRIPTION {List of channel IDs}

add_parameter WR_OPCODE STRING_LIST 0
set_parameter_property WR_OPCODE DISPLAY_NAME {Wr Opcode}
set_parameter_property WR_OPCODE AFFECTS_ELABORATION true
set_parameter_property WR_OPCODE AFFECTS_GENERATION true
set_parameter_property WR_OPCODE HDL_PARAMETER false
set_parameter_property WR_OPCODE DERIVED false
#set_parameter_property WR_OPCODE GROUP "Write_Table"
set_parameter_property WR_OPCODE DISPLAY_HINT "hexadecimal"
set_parameter_property WR_OPCODE DESCRIPTION {List of opcode}

add_parameter WR_EXTEND STRING_LIST 0
set_parameter_property WR_EXTEND DISPLAY_NAME {Wr Extend}
set_parameter_property WR_EXTEND AFFECTS_ELABORATION true
set_parameter_property WR_EXTEND AFFECTS_GENERATION true
set_parameter_property WR_EXTEND HDL_PARAMETER false
set_parameter_property WR_EXTEND DERIVED false
set_parameter_property WR_EXTEND DISPLAY_HINT "hexadecimal"
set_parameter_property WR_EXTEND DESCRIPTION {List of opcode}

add_parameter POLL_OPCODE STRING 0
set_parameter_property POLL_OPCODE DISPLAY_NAME "Poll opcode"
set_parameter_property POLL_OPCODE AFFECTS_ELABORATION true
set_parameter_property POLL_OPCODE AFFECTS_GENERATION true
set_parameter_property POLL_OPCODE HDL_PARAMETER false
set_parameter_property POLL_OPCODE DERIVED false
set_parameter_property POLL_OPCODE DISPLAY_HINT "hexadecimal"
set_parameter_property POLL_OPCODE DESCRIPTION "Opcode used for polling"

add_display_item "Write_Table" WR_ID parameter
add_display_item "Write_Table" WR_OPERATION parameter
add_display_item "Write_Table" WR_OPCODE parameter
add_display_item "Write_Table" WR_EXTEND parameter
add_display_item "Write_Table" WR_ADDR_BYTES parameter

#----------------------------------------------------------------------

#----------------------------------------------------------------------
add_parameter RD_ID STRING_LIST 1
set_parameter_property RD_ID DISPLAY_NAME {No}
set_parameter_property RD_ID AFFECTS_ELABORATION true
set_parameter_property RD_ID AFFECTS_GENERATION true
set_parameter_property RD_ID HDL_PARAMETER false
set_parameter_property RD_ID DERIVED true
set_parameter_property RD_ID GROUP "Read_Table"
set_parameter_property RD_ID DESCRIPTION {List of IDs}

add_parameter RD_OPERATION STRING_LIST "01"
set_parameter_property RD_OPERATION DISPLAY_NAME {Operation} 
set_parameter_property RD_OPERATION AFFECTS_ELABORATION true
set_parameter_property RD_OPERATION AFFECTS_GENERATION true
set_parameter_property RD_OPERATION HDL_PARAMETER false
set_parameter_property RD_OPERATION DERIVED true
set_parameter_property RD_OPERATION GROUP "Read_Table"
set_parameter_property RD_OPERATION DESCRIPTION {List of channel IDs}

add_parameter RD_OPCODE STRING_LIST 0
set_parameter_property RD_OPCODE DISPLAY_NAME {Opcode}
set_parameter_property RD_OPCODE AFFECTS_ELABORATION true
set_parameter_property RD_OPCODE AFFECTS_GENERATION true
set_parameter_property RD_OPCODE HDL_PARAMETER false
set_parameter_property RD_OPCODE DERIVED false
set_parameter_property RD_OPCODE GROUP "Read_Table"
set_parameter_property RD_OPCODE DISPLAY_HINT "hexadecimal"
set_parameter_property RD_OPCODE DESCRIPTION {List of opcode}

add_parameter RD_EXTEND STRING_LIST 0
set_parameter_property RD_EXTEND DISPLAY_NAME {Opcode}
set_parameter_property RD_EXTEND AFFECTS_ELABORATION true
set_parameter_property RD_EXTEND AFFECTS_GENERATION true
set_parameter_property RD_EXTEND HDL_PARAMETER false
set_parameter_property RD_EXTEND DERIVED false
set_parameter_property RD_EXTEND GROUP "Read_Table"
set_parameter_property RD_EXTEND DISPLAY_HINT "hexadecimal"
set_parameter_property RD_EXTEND DESCRIPTION {List of opcode}

add_parameter RD_DUMMY_BYTES STRING_LIST 0
set_parameter_property RD_DUMMY_BYTES DISPLAY_NAME {Dummy bytes}
set_parameter_property RD_DUMMY_BYTES AFFECTS_ELABORATION true
set_parameter_property RD_DUMMY_BYTES AFFECTS_GENERATION true
set_parameter_property RD_DUMMY_BYTES HDL_PARAMETER false
set_parameter_property RD_DUMMY_BYTES DERIVED false
set_parameter_property RD_DUMMY_BYTES GROUP "Read_Table"
set_parameter_property RD_DUMMY_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property RD_DUMMY_BYTES DESCRIPTION {List of dummy bytes}

add_parameter RW_ADDR_BYTES STRING 0
set_parameter_property RW_ADDR_BYTES DISPLAY_NAME "Read/Write address bytes"
set_parameter_property RW_ADDR_BYTES AFFECTS_ELABORATION true
set_parameter_property RW_ADDR_BYTES AFFECTS_GENERATION true
set_parameter_property RW_ADDR_BYTES HDL_PARAMETER false
set_parameter_property RW_ADDR_BYTES DERIVED false
set_parameter_property RW_ADDR_BYTES DISPLAY_HINT "hexadecimal"
set_parameter_property RW_ADDR_BYTES DESCRIPTION "Byte addressing used for Read/Write operation"

add_parameter EXTRA_EN STRING 0
set_parameter_property EXTRA_EN DISPLAY_NAME {Extra EN}
set_parameter_property EXTRA_EN AFFECTS_ELABORATION true
set_parameter_property EXTRA_EN AFFECTS_GENERATION true
set_parameter_property EXTRA_EN HDL_PARAMETER false
set_parameter_property EXTRA_EN DERIVED true
set_parameter_property EXTRA_EN DESCRIPTION {List of IDs}

add_parameter EXTRA_ID STRING_LIST 1
set_parameter_property EXTRA_ID DISPLAY_NAME {Extra ID}
set_parameter_property EXTRA_ID AFFECTS_ELABORATION true
set_parameter_property EXTRA_ID AFFECTS_GENERATION true
set_parameter_property EXTRA_ID HDL_PARAMETER false
set_parameter_property EXTRA_ID DERIVED true
set_parameter_property EXTRA_ID DESCRIPTION {List of IDs}

add_parameter EXTRA_OPERATION STRING_LIST "01"
set_parameter_property EXTRA_OPERATION DISPLAY_NAME {Extra Operation} 
set_parameter_property EXTRA_OPERATION AFFECTS_ELABORATION true
set_parameter_property EXTRA_OPERATION AFFECTS_GENERATION true
set_parameter_property EXTRA_OPERATION HDL_PARAMETER false
set_parameter_property EXTRA_OPERATION DERIVED true
set_parameter_property EXTRA_OPERATION DESCRIPTION {List of channel IDs}


#----------------------------------------------------------------------

# +-----------------------------------
# | Callback procedure for parameters
# +-----------------------------------
proc update_opcode {} {
    #set com_numb_supported 			[get_parameter_value COM_ID_LIST]
    #set com_operation_supported 	[get_parameter_value COM_OPERATION_LIST]
    
    #set com_numb_supported 			{0 1 2 3 4 5 6 7 8}
	#set com_operation_supported 	{"wr_enable" "wr_disable" "wr_status" "rd_status" "sector_erase" "subsector_erase" "isr" "ier" "chip_select"}
	
	# QSPI that supported for dual die - die_erase
    set supported_stack_die {"EPCQL512" "EPCQL1024" "N25Q512" "N25Q00A" "MT25QU01G"}
	# QSPI that supported for 4-byte addressing - en4b_addr, ex4b_addr
    set supported_4byte_addr {"EPCQ256" "EPCQ512" "EPCQL256" "EPCQL512" "EPCQL1024" "N25Q256" "N25Q512" "N25Q00A" "MT25QL512A" "MT25QL256" "MT25QL512" "MT25QL512A" "MT25QU256" "MT25QU512"}
	set is_stack_die            "false"
	set is_4byte_addr_support "false"
	set get_flash_type         [get_parameter_value FLASH_TYPE]	
	set com_numb_supported 			{0 1 2 3 4 5}
	set flash_manufacturer          [proc_get_device_manufacturer [ get_parameter_value FLASH_TYPE ]]
	set_parameter_value FLASH_VENDOR	$flash_manufacturer

	# debug
	#send_message {info text} "get_flash_type : $get_flash_type"	
	#send_message {info text} "flash_manufacturer : $flash_manufacturer"	
	 # check whether SPI is stacked die
    foreach re_spi   $supported_stack_die {
        if {$re_spi eq $get_flash_type} {
            set is_stack_die    "true"
            break;
         }
     }
	 
	 # check whether SPI device support 4-byte addressing
    foreach re_spi   $supported_4byte_addr {
        if {$re_spi eq $get_flash_type} {
            set is_4byte_addr_support   "true"
            break;
         }
     }
	set misc_numb_supported 		{13 14 15 16 17 18 19 20 21 22}
	set extra_numb_supported 		{23 24 25 26 27}
	
	if {$flash_manufacturer eq "ALTERA"} {
        set misc_operation_supported 	{"wr_NVCR" "rd_NVCR" "rd_flag_status" "clr_flag_status" }
        set extra_en 					0
    	set extra_operation_supported 	{"NA" "NA" "NA" "NA" "NA"}
    } elseif {$flash_manufacturer eq "MICRON"} {

    	set misc_operation_supported 	{"wr_NVCR" "rd_NVCR" "rd_flag_status" "clr_flag_status" }
    	set extra_en 					1
    	set extra_operation_supported 	{"device_id_data_0" "device_id_data_1" "device_id_data_2" "device_id_data_3" "device_id_data_4"}

    } elseif {$flash_manufacturer eq "SPANSION"} {
    	set misc_operation_supported 	{"NA" "NA" "NA" "NA" }
    	set extra_en 					1
    	set extra_operation_supported 	{"device_id_data_0" "device_id_data_1" "device_id_data_2" "device_id_data_3" "device_id_data_4"  }
    
    } elseif {$flash_manufacturer eq "ALTERA_EPCQA"} {
    	set misc_operation_supported 	{"NA" "NA" "NA" "NA" }
    	set extra_en 					0
    	set extra_operation_supported 	{"device_id_data_0" "device_id_data_1" "device_id_data_2" "device_id_data_3" "device_id_data_4"  }
    }

    if {$flash_manufacturer eq "ALTERA_EPCQA"} {
		set com_operation_supported 	{"wr_enable" "wr_disable" "wr_status" "rd_status" "block_erase" "sector_erase"}
    } else {
    	set com_operation_supported 	{"wr_enable" "wr_disable" "wr_status" "rd_status" "sector_erase" "subsector_erase"}
    }

	if { $is_stack_die } {
		lappend  misc_operation_supported "NA" "die_erase"
	} else {
		lappend  misc_operation_supported "bulk_erase/chip_erase" "NA"
	}
	if { $is_4byte_addr_support } {
		lappend  misc_operation_supported "4bytes_addr_en" "4bytes_addr_ex"
	} else {
		lappend  misc_operation_supported "NA" "NA"
	}
	
	lappend  misc_operation_supported "sector_protect"
	if {$flash_manufacturer eq "ALTERA" || $flash_manufacturer eq "ALTERA_EPCQA"} {
		lappend  misc_operation_supported "rd_memory_capacity_id"
	} else {
		lappend  misc_operation_supported "rd_device_id"
	}

	set wr_numb_supported 			[get_parameter_value WR_ID_LIST]
    set wr_operation_supported 		[get_parameter_value WR_OPERATION_LIST]
	set wr_opcode_defined 			[get_parameter_value WR_OPCODE_LIST]
	set rd_numb_supported 			[get_parameter_value RD_ID_LIST]
    set rd_operation_supported 		[get_parameter_value RD_OPERATION_LIST]
	set rd_opcode_defined 			[get_parameter_value RD_OPCODE_LIST]

	# There extra is for read device ID and runtime operation. These register offset
	# do not need predefined opcode information
	set_parameter_value EXTRA_EN 			$extra_en
	set_parameter_value EXTRA_ID 			$extra_numb_supported
    set_parameter_value EXTRA_OPERATION 	$extra_operation_supported

	#pass values back to the table
	set_parameter_value COM_ID 			$com_numb_supported
    set_parameter_value COM_OPERATION 	$com_operation_supported

	set_parameter_value MISC_ID 		$misc_numb_supported
    set_parameter_value MISC_OPERATION 	$misc_operation_supported
    set_parameter_value WR_ID 			$wr_numb_supported
    set_parameter_value WR_OPERATION 	$wr_operation_supported
    set_parameter_value RD_ID 			$rd_numb_supported
    set_parameter_value RD_OPERATION 	$rd_operation_supported

    # Gather information about the common Operations
    #set common_op_offset 			[get_parameter_value COM_ID]
    #set common_op_name   			[get_parameter_value COM_OPERATION]
    #set common_op_opcode 			[get_parameter_value COM_OPCODE]
    #set common_op_addr_bytes    	[get_parameter_value COM_ADDR_BYTES]
    #set common_op_data_in_bytes 	[get_parameter_value COM_DATA_IN_BYTES]
    #set common_op_data_out_bytes 	[get_parameter_value COM_DATA_OUT_BYTES]
    #set common_op_dummy_bytes     	[get_parameter_value COM_DUMMY_BYTES]

	#set misc_op_offset 			[get_parameter_value MISC_ID]
    #set misc_op_name   			[get_parameter_value MISC_OPERATION]
    #set misc_op_opcode 			[get_parameter_value MISC_OPCODE]
    #set misc_op_addr_bytes    	[get_parameter_value MISC_ADDR_BYTES]
    #set misc_op_data_in_bytes 	[get_parameter_value MISC_DATA_IN_BYTES]
    #set misc_op_data_out_bytes 	[get_parameter_value MISC_DATA_OUT_BYTES]
    #set misc_op_dummy_bytes     [get_parameter_value MISC_DUMMY_BYTES]

    #send_message {info text} "common_op_offset : 			$common_op_offset"
    #send_message {info text} "common_op_name : 				$common_op_name"
    #send_message {info text} "common_op_opcode : 			$common_op_opcode"
    #send_message {info text} "common_op_addr_bytes : 		$common_op_addr_bytes"
    #send_message {info text} "common_op_data_in_bytes : 	$common_op_data_in_bytes"
    #send_message {info text} "common_op_data_out_bytes : 	$common_op_data_out_bytes"
    #send_message {info text} "common_op_dummy_bytes : 		$common_op_dummy_bytes"

	#send_message {info text} "misc_op_offset : 				$misc_op_offset"
	#send_message {info text} "misc_op_name : 				$misc_op_name"
	#send_message {info text} "misc_op_opcode : 				$misc_op_opcode"
	#send_message {info text} "misc_op_addr_bytes : 			$misc_op_addr_bytes"
	#send_message {info text} "misc_op_data_in_bytes : 		$misc_op_data_in_bytes"
	#send_message {info text} "misc_op_data_out_bytes : 		$misc_op_data_out_bytes"
	#send_message {info text} "misc_op_dummy_bytes : 		$misc_op_dummy_bytes"
}

proc proc_get_device_manufacturer {flash_type} {
    if {[ string match "*EPCQ*A*" "$flash_type" ]} {
    	return "ALTERA_EPCQA"
	} elseif {[ string match "*N25*" "$flash_type" ] || [ string match "*MT25Q*" "$flash_type" ]} {
		return "MICRON"
	} elseif {[ string match "*S25*" "$flash_type" ]} {
		return "SPANSION"
	} elseif {[ string match "*EPCQ*" "$flash_type" ] || [ string match "*EPCS*" "$flash_type" ]} {
		return "ALTERA"
	}
}
