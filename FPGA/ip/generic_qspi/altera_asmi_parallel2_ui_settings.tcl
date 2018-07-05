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
# | $Header: //acds/rel/18.0std/ip/pgm/altera_asmi_parallel2/asmi_top/altera_asmi_parallel2_ui_settings.tcl#1 $
# | 
# +-----------------------------------


# +-----------------------------------
# | General procedures to check supported parameters
# +-----------------------------------
proc general_parameters_procedure {flag}    {
    # devices that supported simulation
    set supported_sim_devices_list {"Arria 10" "Cyclone V" "Arria V GZ" "Arria V" "Stratix V"}
    set enable_sim_model        [get_parameter_value ENABLE_SIM_MODEL]
    # --- list of variables --- #
    set get_device_setting      [get_parameter_value DEVICE_FAMILY]

    set is_sim_devices_list     "false"
    # check whether devices supporting simulation
    if {[check_device_family_equivalence $get_device_setting $supported_sim_devices_list]} {
        set is_sim_devices_list "true"
     }
    # +-----------------------------------
    # --- VALIDATION CALLBACK --- #
    # +-----------------------------------
    if {$flag eq "validate_settings"} {

       
        set_parameter_value     ENABLE_4BYTE_ADDR   [ proc_get_derive_enable_2byte_addr  [ get_parameter_value FLASH_TYPE ] ]
    
        if { [ get_parameter_value ENABLE_4BYTE_ADDR ] } {
            set_parameter_value ASMI_ADDR_WIDTH 32
        } else {
            set_parameter_value ASMI_ADDR_WIDTH 24
        }

        # --- ENABLE_SIM --- #
        if {$is_sim_devices_list eq "true"} {
            set_parameter_property  ENABLE_SIM_MODEL  ENABLED     true
        } else {
            set_parameter_property  ENABLE_SIM_MODEL  ENABLED     false
        }

        if {$is_sim_devices_list eq "false" && $enable_sim_model eq "true"} {
            send_message error "$get_device_setting does not support enable_sim parameter"
        } 
    }
}


    

