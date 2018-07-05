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


# +-----------------------------------
# | 
# $Id: //acds/rel/18.0std/ip/pgm/altera_asmi_parallel2_top/altera_asmi_parallel2_top_hw_proc.tcl#1 $
# $Revision: #1 $
# $Date: 2018/01/31 $
# $Author: psgswbuild $
# | 
# +-----------------------------------


# +-----------------------------------
# | Set UI Interface during elaboration callback
# +-----------------------------------
proc validation {}     {

    # --- check ini for hidden devices --- #
    set enable_N25Q512      [get_quartus_ini pgm_allow_n25q512 ENABLED]
    set enable_MT25Q512     [get_quartus_ini pgm_allow_mt25q512 ENABLED]
    set enable_MT25Q256     [get_quartus_ini pgm_allow_mt25q256 ENABLED]
    set enable_S25FL127S    [get_quartus_ini pgm_allow_qspi128 ENABLED]
    set enable_MT25Q        [get_quartus_ini pgm_allow_mt25q ENABLED]
    set get_spi_list        [get_parameter_property FLASH_TYPE   ALLOWED_RANGES]
    if {$enable_MT25Q == 1} {
        lappend get_spi_list    "MT25QL256"
        lappend get_spi_list    "MT25QL512"
        lappend get_spi_list    "MT25QU256"
        lappend get_spi_list    "MT25QU512"
        lappend get_spi_list    "MT25QU01G"
    }
    
    if {$enable_MT25Q256 == 1} {
        lappend get_spi_list    "MT25Q256"
    }
    
    if {$enable_MT25Q512 == 1} {
        lappend get_spi_list    "MT25Q512"
    }
     
    if {$enable_N25Q512 == 1} {
        lappend get_spi_list    "N25Q512"
    }
    
    if {$enable_S25FL127S == 1} {
        lappend get_spi_list    "S25FL127S"
    }
    set_parameter_property  FLASH_TYPE   ALLOWED_RANGES      $get_spi_list
    # If ECPQ4A, then QUAD mode does not support
    set flash_type              [ get_parameter_value FLASH_TYPE ]
    if { $flash_type eq "EPCQ4A"} {
        set_parameter_property  DATA_WIDTH   ALLOWED_RANGES  {"NORMAL" "STANDARD" "DUAL"}
    } else {
        set_parameter_property  DATA_WIDTH   ALLOWED_RANGES  {"NORMAL" "STANDARD" "DUAL" "QUAD"}
    }
}

proc compose { } {
    set device_family           [ get_parameter_value DEVICE_FAMILY ]
    set flash_type              [ get_parameter_value FLASH_TYPE ]

    set disable_asmiblock       [get_parameter_value USE_ASMIBLOCK]
    set use_gpio                [get_parameter_value USE_GPIO]
    set io_mode                 [get_parameter_value DATA_WIDTH]
    set enable_chip_select      [get_parameter_value CHIP_SELECT_EN]
    set enable_sim_model        [get_parameter_value ENABLE_SIM_MODEL]

    set pipe_xip                [get_parameter_value PIPE_XIP]
    set pipe_csr                [get_parameter_value PIPE_CSR]
    set pipe_cmd_gen_cmd        [get_parameter_value PIPE_CMD_GEN_CMD]
    set pipe_mux_cmd            [get_parameter_value PIPE_MUX_CMD]
    set user_ncs_num            [get_parameter_value NCS_NUM]

    # check whether devices supporting multiple flash - only for Arria 10, or QSPI interface alone
    if {$use_gpio eq "true" || [check_device_family_equivalence $device_family "Arria 10"] || [check_device_family_equivalence $device_family "Cyclone 10 GX"]} {
        set is_multi_flash_support  1
        set_parameter_property NCS_NUM ENABLED true
    } else {
        set is_multi_flash_support  0
        set_parameter_property NCS_NUM ENABLED false
    }
    
    # check enable flash parameter
    if {$enable_sim_model eq "true"} {
        set enable_flash  1
    } else {
        set enable_flash  0
    }

    # For S10, it only supports GPIO pins 
    if {[check_device_family_equivalence $device_family "Stratix 10"] || [check_device_family_equivalence $device_family "Max 10"]} {
        if {$use_gpio eq "false"} {
            send_message error "$device_family only supported SPI pins interface mode"
        }
     }

    # Devices that supported QSPI - Quad/Dual data width, asmi_dataout, asmi_sdoin, asmi_dataoe - ASMIhardblock
    # If not using GPIO and use asmiblock, old devices can only use NORMAL or STANDARD, even if they export the 
    # ASMI signals, it also goes with that setting.
    # Check whether devices supporting QSPI
    set supported_QSPI_devices_list {"Cyclone 10 GX" "Arria 10" "Cyclone V" "Arria V GZ" "Arria V" "Stratix V" "Max 10"}
    set is_qspi_devices_list    "false"
    if {[check_device_family_equivalence $device_family $supported_QSPI_devices_list]} {
        set is_qspi_devices_list    "true"
    }

    if {$use_gpio eq "false"} {
        if {$is_qspi_devices_list eq "false"} {
            if {$io_mode eq "DUAL" || $io_mode eq "QUAD"} {
                send_message error "$device_family only supported for NORMAL/STANDARD data width"
            }
        }
    }

    # Instances and instance parameters
    # (disabled instances are intentionally culled)
    add_instance clk_asmi2_core clock_source 18.0
    #set_instance_parameter_value clk_asmi2_core {clockFrequency} {50000000.0}
    #set_instance_parameter_value clk_asmi2_core {clockFrequencyKnown} {0}
    set_instance_parameter_value clk_asmi2_core {resetSynchronousEdges} {NONE}

    add_instance asmi2_core altera_asmi_parallel2 18.0
    set_instance_parameter_value asmi2_core {DATA_WIDTH} $io_mode
    set_instance_parameter_value asmi2_core {gui_use_asmiblock} $disable_asmiblock
    set_instance_parameter_value asmi2_core {gui_use_gpio} $use_gpio
    set_instance_parameter_value asmi2_core {NCS_NUM} $user_ncs_num
    set_instance_parameter_value asmi2_core {ENABLE_SIM_MODEL} $enable_flash
    # If A10 or Cyclone 10 GX, set 3 to number of chip 
    if {[check_device_family_equivalence $device_family "Arria 10"] || [check_device_family_equivalence $device_family "Cyclone 10 GX"]} {
        set_instance_parameter_value asmi2_core {CHIP_SELS} 3
    } else {
        set_instance_parameter_value asmi2_core {CHIP_SELS} $user_ncs_num     
    }
    set_instance_parameter_value asmi2_core {DEBUG_OPTION} {0}
    set_instance_parameter_value asmi2_core {USE_CHIP_SEL_FROM_CSR} {1}
    add_instance asmi2_addr_adaption avl_mem_addr_adaption 18.0
    set_instance_parameter_value asmi2_addr_adaption {FLASH_TYPE} $flash_type

    # connections and connection parameters
    add_connection clk_asmi2_core.clk asmi2_core.clk clock
    add_connection clk_asmi2_core.clk_reset asmi2_core.reset reset
    add_connection clk_asmi2_core.clk asmi2_addr_adaption.clock_sink clock
    add_connection clk_asmi2_core.clk_reset asmi2_addr_adaption.reset reset

    # exported interfaces
    add_interface clk clock sink
    set_interface_property clk EXPORT_OF clk_asmi2_core.clk_in
    add_interface reset reset sink
    set_interface_property reset EXPORT_OF clk_asmi2_core.clk_in_reset
    add_interface avl_csr avalon slave
    set_interface_property avl_csr EXPORT_OF asmi2_core.avl_csr
    add_interface avl_mem avalon slave
    set_interface_property avl_mem EXPORT_OF asmi2_addr_adaption.avl_mem
    add_connection asmi2_addr_adaption.asmi_mem asmi2_core.avl_mem avalon
    #set_interface_property avl_mem EXPORT_OF asmi2_core.avl_mem

    # make the gpio is higher priroty, if this is check, then disable_asmiblock has no effect
    if {$use_gpio eq "true"} {
        add_interface qspi_pins conduit end
        set_interface_property qspi_pins EXPORT_OF asmi2_core.qspi_pins
    } elseif {$disable_asmiblock eq "true"} {
        add_interface atom_ports conduit end
        set_interface_property atom_ports EXPORT_OF asmi2_core.atom_ports
    }

    #send_message {info text} "asmi top get_flash_type : $flash_type" 
    if { $flash_type eq "N25Q16"} {     #Micron
        apply_instance_preset   asmi2_core            N25Q016
    } elseif { $flash_type eq "N25Q32"} {   #Micron
        apply_instance_preset   asmi2_core            N25Q032
    } elseif { $flash_type eq "N25Q64"} {   #Micron
        apply_instance_preset   asmi2_core            N25Q064
    } elseif { $flash_type eq "N25Q128"} {  #Micron
        apply_instance_preset   asmi2_core            N25Q128
    } elseif { $flash_type eq "N25Q256"} {  #Micron
        apply_instance_preset   asmi2_core            N25Q256
    } elseif { $flash_type eq "N25Q256-L"} {    #Micron
        apply_instance_preset   asmi2_core            N25Q256
    } elseif { $flash_type eq "N25Q512"} {  #Micron
        apply_instance_preset   asmi2_core            MT25QL512A
    } elseif { $flash_type eq "N25Q512-L" || $flash_type eq "Micron512"} {  #Micron
        apply_instance_preset   asmi2_core            N25Q512
    } elseif { $flash_type eq "N25Q1024-L"} {   #Micron
        apply_instance_preset   asmi2_core            N25Q00A
    } else {                                #Altera EPCQ
        apply_instance_preset           asmi2_core    ${flash_type}
    }

}
