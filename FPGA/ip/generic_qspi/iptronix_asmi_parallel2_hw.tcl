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
# | $Header: //acds/rel/18.0std/ip/pgm/altera_asmi_parallel2/asmi_top/altera_asmi_parallel2_hw.tcl#1 $
# | 
# +-----------------------------------

# request TCL package
package require -exact qsys 16.0
package require -exact altera_terp 1.0

# +-----------------------------------
# | module ALTASMI_PARALLEL
# +-----------------------------------
set_module_property NAME                            iptronix_asmi_parallel2
set_module_property AUTHOR                          "Intel Corporation"
set_module_property DATASHEET_URL                   "http://www.altera.com/literature/ug/ug_altasmi_parallel.pdf"
set_module_property DESCRIPTION                     "The Altera ASMI Parallel megafunction provides access to erasable \
                                                        programmable configurable serial (EPCS) and quad-serial configuration \
                                                        (EPCQ) devices through parallel data input and output ports."
set_module_property DISPLAY_NAME                    "Altera ASMI Parallel II Core"
set_module_property EDITABLE                        false
set_module_property VERSION                         18.0
set_module_property GROUP                           "Basic Functions/Configuration and Programming"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE    true
set_module_property INTERNAL                        true

set_module_property     VALIDATION_CALLBACK     validation
set_module_property     COMPOSITION_CALLBACK    compose
add_display_item "" "General" GROUP tab
add_display_item "" "Simulation" GROUP tab
add_display_item "" "Settings" GROUP tab

# Source files
source iptronix_asmi_parallel2_hw_proc.tcl
source altera_asmi_parallel2_ui_settings.tcl
source altera_asmi_parallel2_ui_configuration.tcl

# +-----------------------------------
# | device family info
# +-----------------------------------
set all_supported_device_families_list {"Max 10" "Arria 10" "Cyclone V" "Arria V GZ" "Arria V" "Stratix V" "Stratix IV" \
                                            "Cyclone IV GX" "Cyclone IV E" "Cyclone III GL" "Arria II GZ" "Arria II GX" "Cyclone 10 GX" "Cyclone 10 LP" "Stratix 10"}
                                    
proc check_device_ini {device_families_list}     {
    set enable_max10    [get_quartus_ini enable_max10_active_serial ENABLED]
  
    if {$enable_max10 == 1} {
        lappend device_families_list    "MAX 10 FPGA"
     } 
    return $device_families_list
}

set device_list    [check_device_ini $all_supported_device_families_list]
set_module_property SUPPORTED_DEVICE_FAMILIES    $device_list

add_parameter           DEVICE_FAMILY   STRING
set_parameter_property  DEVICE_FAMILY   SYSTEM_INFO     {DEVICE_FAMILY}
set_parameter_property  DEVICE_FAMILY   VISIBLE         false
set_parameter_property  DEVICE_FAMILY   HDL_PARAMETER   false

add_parameter           INTENDED_DEVICE_FAMILY  STRING
set_parameter_property  INTENDED_DEVICE_FAMILY  SYSTEM_INFO     {DEVICE_FAMILY}
set_parameter_property  INTENDED_DEVICE_FAMILY  VISIBLE         false

add_parameter           FLASH_VENDOR    STRING              "ALTERA"
set_parameter_property  FLASH_VENDOR    DISPLAY_NAME        "FLASH_VENDOR"
set_parameter_property  FLASH_VENDOR    DERIVED true

# +-----------------------------------
# | Parameters - General tab
# +-----------------------------------
# check elaboration procedure for hidden devices
set all_supported_SPI_list {"EPCS1" "EPCS4" "EPCS16" "EPCS64" "EPCS128" "EPCQ16" "EPCQ32" "EPCQ64" "EPCQ128" "EPCQ4A" "EPCQ16A" "EPCQ32A" "EPCQ64A" "EPCQ128A" "EPCQ256" \
                            "EPCQ512" "EPCQL256" "EPCQL512" "EPCQL1024" "N25Q016" "N25Q016" "N25Q032" "N25Q064" "N25Q128" \
							"N25Q256" "N25Q512" "N25Q00A" "MT25QL512A" "S25FL127S" "MT25QL256" "MT25QL512" "MT25QU256" \
                            "MT25QU512" "MT25QU01G"}


# --- Parameters for HDL parameters generation --- #
# SPI device selection

add_parameter           FLASH_TYPE   STRING              "EPCQ128"
set_parameter_property  FLASH_TYPE   DISPLAY_NAME        "Configuration device type"
set_parameter_property  FLASH_TYPE   ALLOWED_RANGES      $all_supported_SPI_list
set_parameter_property  FLASH_TYPE   DESCRIPTION         "Select targeted devices"
set_parameter_property  FLASH_TYPE   AFFECTS_GENERATION  true
add_display_item        "General"   FLASH_TYPE           parameter

# Data width
add_parameter           DATA_WIDTH          STRING              "STANDARD"
set_parameter_property  DATA_WIDTH          DISPLAY_NAME        "Choose I/O mode"
set_parameter_property  DATA_WIDTH          ALLOWED_RANGES      {"NORMAL" "STANDARD" "DUAL" "QUAD"}
set_parameter_property  DATA_WIDTH          DESCRIPTION         "Select extended data width when Fast Read operation is enabled"
set_parameter_property  DATA_WIDTH          AFFECTS_GENERATION  true
add_display_item        "General"           DATA_WIDTH          parameter

# use asmiblock - currently is invert. when check means not using ASMIBLOCK, so a bit confusing. Need to change the parameter name when updating presets.
add_parameter           gui_use_asmiblock               BOOLEAN             0
set_parameter_property  gui_use_asmiblock               DISPLAY_NAME        "Disable dedicated Active Serial interface"
set_parameter_property  gui_use_asmiblock               DESCRIPTION         "Check to route ASMIBLOCK signals to top level of design"
set_parameter_property  gui_use_asmiblock               AFFECTS_GENERATION  true
add_display_item        "General"                       gui_use_asmiblock       parameter

# use gpio
add_parameter           gui_use_gpio    BOOLEAN             0
set_parameter_property  gui_use_gpio    DISPLAY_NAME        "Enable SPI pins interface"
set_parameter_property  gui_use_gpio    DESCRIPTION         "Check to translate ASMIBLOCK signals to SPI pins interface"
set_parameter_property  gui_use_gpio    AFFECTS_GENERATION  true
add_display_item        "General"       gui_use_gpio       parameter

# Check the number of ncs connected to flash
add_parameter           NCS_NUM     INTEGER             1
set_parameter_property  NCS_NUM     DISPLAY_NAME        "Number of Chip Select"
set_parameter_property  NCS_NUM     ALLOWED_RANGES      {1:16}
set_parameter_property  NCS_NUM     DESCRIPTION         "Select the number of chip select connected to flash"
set_parameter_property  NCS_NUM     AFFECTS_GENERATION  true
add_display_item        "General"       NCS_NUM       parameter

# +-----------------------------------
# | Parameters - Simulation tab
# +-----------------------------------
# enable_sim
add_parameter           ENABLE_SIM_MODEL  BOOLEAN             0
set_parameter_property  ENABLE_SIM_MODEL  DISPLAY_NAME        "Enable simulation"
set_parameter_property  ENABLE_SIM_MODEL  DESCRIPTION         "Check to enable simulation for Active Serial pins"
set_parameter_property  ENABLE_SIM_MODEL  AFFECTS_GENERATION  true
add_display_item        "Simulation"    ENABLE_SIM_MODEL      parameter

add_parameter ASMI_ADDR_WIDTH INTEGER 24
set_parameter_property ASMI_ADDR_WIDTH DEFAULT_VALUE 24
set_parameter_property ASMI_ADDR_WIDTH DISPLAY_NAME ASMI_ADDR_WIDTH
set_parameter_property ASMI_ADDR_WIDTH DERIVED true
set_parameter_property ASMI_ADDR_WIDTH TYPE INTEGER
set_parameter_property ASMI_ADDR_WIDTH VISIBLE false
set_parameter_property ASMI_ADDR_WIDTH UNITS None
set_parameter_property ASMI_ADDR_WIDTH ALLOWED_RANGES {24, 32}      
set_parameter_property ASMI_ADDR_WIDTH HDL_PARAMETER false

add_parameter ENABLE_4BYTE_ADDR INTEGER "0"
set_parameter_property ENABLE_4BYTE_ADDR DISPLAY_NAME "Enable 4-byte addressing mode"
set_parameter_property ENABLE_4BYTE_ADDR DESCRIPTION "Check to enable 4-byte addressing mode for device larger than 128Mbyte"
set_parameter_property ENABLE_4BYTE_ADDR AFFECTS_GENERATION true
set_parameter_property ENABLE_4BYTE_ADDR VISIBLE false
set_parameter_property ENABLE_4BYTE_ADDR HDL_PARAMETER false
set_parameter_property ENABLE_4BYTE_ADDR DERIVED true

add_parameter CHIP_SELS INTEGER "1"
set_parameter_property CHIP_SELS DISPLAY_NAME "Number of Chip Selects used"
set_parameter_property CHIP_SELS ALLOWED_RANGES {1 2 3}
set_parameter_property CHIP_SELS DESCRIPTION "Number of EPCQ(L) devices that are attached and need a CHIPSEL"
set_parameter_property CHIP_SELS HDL_PARAMETER false
set_parameter_property CHIP_SELS AFFECTS_GENERATION true

# +-----------------------------------
# | Debug Parameters
# +----------------------------------- 

add_parameter               USE_CHIP_SEL_FROM_CSR INTEGER 0
set_parameter_property      USE_CHIP_SEL_FROM_CSR DISPLAY_NAME "Use Chip Select From CSR"
set_parameter_property      USE_CHIP_SEL_FROM_CSR UNITS None
set_parameter_property      USE_CHIP_SEL_FROM_CSR ALLOWED_RANGES { "0: Use dedicated Chip Select for XIP" "1: Use Chip Select from CSR"}
set_parameter_property      USE_CHIP_SEL_FROM_CSR AFFECTS_ELABORATION true
set_parameter_property      USE_CHIP_SEL_FROM_CSR AFFECTS_GENERATION true
set_parameter_property      USE_CHIP_SEL_FROM_CSR DERIVED false
set_parameter_property      USE_CHIP_SEL_FROM_CSR HDL_PARAMETER false
add_display_item        "General"   USE_CHIP_SEL_FROM_CSR       parameter


add_parameter               DEBUG_OPTION INTEGER 0
set_parameter_property      DEBUG_OPTION DISPLAY_NAME "Debug Options"
set_parameter_property      DEBUG_OPTION UNITS None
set_parameter_property      DEBUG_OPTION ALLOWED_RANGES { "0:None" "1:Two controllers only - 32 bits" "2:Two Controller and Command Generator - 8 bits"}
set_parameter_property      DEBUG_OPTION AFFECTS_ELABORATION true
set_parameter_property      DEBUG_OPTION AFFECTS_GENERATION true
set_parameter_property      DEBUG_OPTION DERIVED false
set_parameter_property      DEBUG_OPTION HDL_PARAMETER false
add_display_item        "General"   DEBUG_OPTION       parameter

add_parameter               PIPE_CSR INTEGER 0
set_parameter_property      PIPE_CSR DISPLAY_NAME "Pipeline CSR output"
set_parameter_property      PIPE_CSR UNITS None
set_parameter_property      PIPE_CSR ALLOWED_RANGES { "0:None" "1:Pipeline"}
set_parameter_property      PIPE_CSR AFFECTS_ELABORATION true
set_parameter_property      PIPE_CSR AFFECTS_GENERATION true
set_parameter_property      PIPE_CSR DERIVED false
set_parameter_property      PIPE_CSR HDL_PARAMETER false
add_display_item        "General"   PIPE_CSR       parameter

add_parameter               PIPE_XIP INTEGER 0
set_parameter_property      PIPE_XIP DISPLAY_NAME "Pipeline XIP output"
set_parameter_property      PIPE_XIP UNITS None
set_parameter_property      PIPE_XIP ALLOWED_RANGES { "0:None" "1:Pipeline"}
set_parameter_property      PIPE_XIP AFFECTS_ELABORATION true
set_parameter_property      PIPE_XIP AFFECTS_GENERATION true
set_parameter_property      PIPE_XIP DERIVED false
set_parameter_property      PIPE_XIP HDL_PARAMETER false
add_display_item        "General"   PIPE_XIP       parameter

add_parameter               PIPE_CMD_GEN_CMD INTEGER 0
set_parameter_property      PIPE_CMD_GEN_CMD DISPLAY_NAME "Pipeline Cmd Generator output"
set_parameter_property      PIPE_CMD_GEN_CMD UNITS None
set_parameter_property      PIPE_CMD_GEN_CMD ALLOWED_RANGES { "0:None" "1:Pipeline"}
set_parameter_property      PIPE_CMD_GEN_CMD AFFECTS_ELABORATION true
set_parameter_property      PIPE_CMD_GEN_CMD AFFECTS_GENERATION true
set_parameter_property      PIPE_CMD_GEN_CMD DERIVED false
set_parameter_property      PIPE_CMD_GEN_CMD HDL_PARAMETER false
add_display_item        "General"   PIPE_CMD_GEN_CMD       parameter

add_parameter               PIPE_MUX_CMD INTEGER 0
set_parameter_property      PIPE_MUX_CMD DISPLAY_NAME "Pipeline Mux Output"
set_parameter_property      PIPE_MUX_CMD UNITS None
set_parameter_property      PIPE_MUX_CMD ALLOWED_RANGES { "0:None" "1:Pipeline"}
set_parameter_property      PIPE_MUX_CMD AFFECTS_ELABORATION true
set_parameter_property      PIPE_MUX_CMD AFFECTS_GENERATION true
set_parameter_property      PIPE_MUX_CMD DERIVED false
set_parameter_property      PIPE_MUX_CMD HDL_PARAMETER false
add_display_item        "General"   PIPE_MUX_CMD       parameter
