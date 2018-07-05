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
# | $Header: //acds/rel/18.0std/ip/pgm/altera_asmi_parallel2/asmi_top/altera_asmi_parallel2_hw_proc.tcl#1 $
# | 
# +-----------------------------------

# +-----------------------------------
# | Validate user selection during validation callback
# +-----------------------------------
proc validation {}  {

    # --- update parameters and ports with regards to selected FPGA/flash device --- #
    general_parameters_procedure validate_settings
    
}

proc ports_transfer {}  {

      set all_ports [get_interface_ports]
      return $all_ports
}

proc proc_get_derive_enable_2byte_addr {flash_type} {
    if { [ string match "*256*" "$flash_type" ] || [ string match "*512*" "$flash_type" ] || [ string match "*1024*" "$flash_type" ] || [ string match "*01G*" "$flash_type" ]} {
        return true
    } else {
        return false
    }
}

proc compose { } {
    update_opcode
    
    set get_flash_vendor              [get_parameter_value FLASH_VENDOR] 
    set common_op_offset            [get_parameter_value COM_ID]
    set common_op_name              [get_parameter_value COM_OPERATION]
    set common_op_opcode            [get_parameter_value COM_OPCODE]
    set common_op_addr_bytes        [get_parameter_value COM_ADDR_BYTES]
    set common_op_data_in_bytes     [get_parameter_value COM_DATA_IN_BYTES]
    set common_op_data_out_bytes    [get_parameter_value COM_DATA_OUT_BYTES]
    set common_op_dummy_bytes       [get_parameter_value COM_DUMMY_BYTES]

    set misc_op_offset          [get_parameter_value MISC_ID]
    set misc_op_name            [get_parameter_value MISC_OPERATION]
    set misc_op_opcode          [get_parameter_value MISC_OPCODE]
    set misc_op_addr_bytes      [get_parameter_value MISC_ADDR_BYTES]
    set misc_op_data_in_bytes   [get_parameter_value MISC_DATA_IN_BYTES]
    set misc_op_data_out_bytes  [get_parameter_value MISC_DATA_OUT_BYTES]
    set misc_op_dummy_bytes     [get_parameter_value MISC_DUMMY_BYTES]

    set extra_op_en            [get_parameter_value EXTRA_EN]          
    set extra_op_offset        [get_parameter_value EXTRA_ID]          
    set extra_op_name          [get_parameter_value EXTRA_OPERATION]   

    set disable_asmiblock       [get_parameter_value gui_use_asmiblock]
    set use_gpio                [get_parameter_value gui_use_gpio]
    set user_ncs_num            [get_parameter_value NCS_NUM]
    set io_mode                 [get_parameter_value DATA_WIDTH]
    set chip_sels               [get_parameter_value CHIP_SELS]
    set write_op_opcode         [get_parameter_value WR_OPCODE]
    set write_extended_mode     [get_parameter_value WR_EXTEND]
    set poll_op_opcode          [get_parameter_value POLL_OPCODE]
    set read_op_opcode          [get_parameter_value RD_OPCODE]
    set read_extended_mode      [get_parameter_value RD_EXTEND]
    set read_dummy_bytes        [get_parameter_value RD_DUMMY_BYTES]
    set rd_wr_byte_addressing   [get_parameter_value RW_ADDR_BYTES]

    set debug_option            [get_parameter_value DEBUG_OPTION]
    set use_chip_sel_from_csr   [get_parameter_value USE_CHIP_SEL_FROM_CSR]
    set pipe_xip                [get_parameter_value PIPE_XIP]
    set pipe_csr                [get_parameter_value PIPE_CSR]
    set pipe_cmd_gen_cmd        [get_parameter_value PIPE_CMD_GEN_CMD]
    set pipe_mux_cmd            [get_parameter_value PIPE_MUX_CMD]
    set enable_sim_model        [get_parameter_value ENABLE_SIM_MODEL]

    # Instances and instance parameters
    # (disabled instances are intentionally culled)
    add_instance csr_controller altera_asmi2_csr_controller 18.0
    set_instance_parameter_value csr_controller {COM_ID}                $common_op_offset
    set_instance_parameter_value csr_controller {COM_OPERATION}         $common_op_name
    set_instance_parameter_value csr_controller {COM_OPCODE}            $common_op_opcode
    set_instance_parameter_value csr_controller {COM_ADDR_BYTES}        $common_op_addr_bytes
    set_instance_parameter_value csr_controller {COM_DATA_IN_BYTES}     $common_op_data_in_bytes
    set_instance_parameter_value csr_controller {COM_DATA_OUT_BYTES}    $common_op_data_out_bytes
    set_instance_parameter_value csr_controller {COM_DUMMY_BYTES}       $common_op_dummy_bytes
    set_instance_parameter_value csr_controller {MISC_ID}               $misc_op_offset
    set_instance_parameter_value csr_controller {MISC_OPERATION}        $misc_op_name
    set_instance_parameter_value csr_controller {MISC_OPCODE}           $misc_op_opcode
    set_instance_parameter_value csr_controller {MISC_ADDR_BYTES}       $misc_op_addr_bytes
    set_instance_parameter_value csr_controller {MISC_DATA_IN_BYTES}    $misc_op_data_in_bytes
    set_instance_parameter_value csr_controller {MISC_DATA_OUT_BYTES}   $misc_op_data_out_bytes
    set_instance_parameter_value csr_controller {MISC_DUMMY_BYTES}      $misc_op_dummy_bytes
    set_instance_parameter_value csr_controller {EXTRA_EN}              $extra_op_en
    set_instance_parameter_value csr_controller {EXTRA_OPERATION}       $extra_op_name
    set_instance_parameter_value csr_controller {EXTRA_ID}              $extra_op_offset
    set_instance_parameter_value csr_controller {WR_OPCODE} {0}
    set_instance_parameter_value csr_controller {WR_ENABLE} {0}
    set_instance_parameter_value csr_controller {WR_DUMMY_BYTES} {0}
    set_instance_parameter_value csr_controller {WR_ADDR_BYTES} {0}
    set_instance_parameter_value csr_controller {RD_OPCODE} {0}
    set_instance_parameter_value csr_controller {RD_ADDR_BYTES} {0}
    set_instance_parameter_value csr_controller {RD_DUMMY_BYTES} {0}
    set_instance_parameter_value csr_controller {RD_ENABLE} {0}

    add_instance xip_controller altera_asmi2_xip_controller 18.0
    set_instance_parameter_value xip_controller {FLASH_VENDOR}      $get_flash_vendor
    set_instance_parameter_value xip_controller {WR_OPCODE}         $write_op_opcode
    set_instance_parameter_value xip_controller {WR_EXTEND}         $write_extended_mode
    set_instance_parameter_value xip_controller {POLL_OPCODE}       $poll_op_opcode
    set_instance_parameter_value xip_controller {RD_OPCODE}         $read_op_opcode
    set_instance_parameter_value xip_controller {RD_EXTEND}         $read_extended_mode
    set_instance_parameter_value xip_controller {RD_DUMMY_BYTES}    $read_dummy_bytes
    set_instance_parameter_value xip_controller {RW_ADDR_BYTES}     $rd_wr_byte_addressing
    set_instance_parameter_value xip_controller {DATA_WIDTH}        $io_mode

    add_instance clk_bridge altera_clock_bridge 18.0
    set_instance_parameter_value clk_bridge {EXPLICIT_CLOCK_RATE} {0.0}
    set_instance_parameter_value clk_bridge {NUM_CLOCK_OUTPUTS} {1}

    add_instance merlin_demultiplexer_0 altera_merlin_demultiplexer 18.0
    set_instance_parameter_value merlin_demultiplexer_0 {ST_DATA_W} {32}
    set_instance_parameter_value merlin_demultiplexer_0 {ST_CHANNEL_W} {2}
    set_instance_parameter_value merlin_demultiplexer_0 {NUM_OUTPUTS} {2}
    set_instance_parameter_value merlin_demultiplexer_0 {VALID_WIDTH} {1}
    set_instance_parameter_value merlin_demultiplexer_0 {MERLIN_PACKET_FORMAT} {}

    add_instance multiplexer altera_merlin_multiplexer 18.0
    set_instance_parameter_value multiplexer {ST_DATA_W} {32}
    set_instance_parameter_value multiplexer {ST_CHANNEL_W} {2}
    set_instance_parameter_value multiplexer {NUM_INPUTS} {2}
    set_instance_parameter_value multiplexer {PIPELINE_ARB} {1}
    set_instance_parameter_value multiplexer {USE_EXTERNAL_ARB} {0}
    set_instance_parameter_value multiplexer {PKT_TRANS_LOCK} {-1}
    #set_instance_parameter_value multiplexer {ARBITRATION_SCHEME} {round-robin}
    set_instance_parameter_value multiplexer {ARBITRATION_SCHEME} {fixed-priority}
    #set_instance_parameter_value multiplexer {ARBITRATION_SHARES} {0 1}
    set_instance_parameter_value multiplexer {MERLIN_PACKET_FORMAT} {}

    add_instance reset_bridge altera_reset_bridge 18.0
    set_instance_parameter_value reset_bridge {ACTIVE_LOW_RESET} {0}
    set_instance_parameter_value reset_bridge {SYNCHRONOUS_EDGES} {none}
    set_instance_parameter_value reset_bridge {NUM_RESET_OUTPUTS} {1}
    set_instance_parameter_value reset_bridge {USE_RESET_REQUEST} {0}

    # add in single stage st PIPELINE
    if {$pipe_csr == 1} {
    add_instance csr_ctrl_pipeline altera_avalon_st_pipeline_stage 18.0
    set_instance_parameter_value csr_ctrl_pipeline {SYMBOLS_PER_BEAT} {1}
    set_instance_parameter_value csr_ctrl_pipeline {BITS_PER_SYMBOL} {32}
    set_instance_parameter_value csr_ctrl_pipeline {USE_PACKETS} {1}
    set_instance_parameter_value csr_ctrl_pipeline {USE_EMPTY} {0}
    set_instance_parameter_value csr_ctrl_pipeline {CHANNEL_WIDTH} {2}
    set_instance_parameter_value csr_ctrl_pipeline {MAX_CHANNEL} {0}
    set_instance_parameter_value csr_ctrl_pipeline {ERROR_WIDTH} {0}
    set_instance_parameter_value csr_ctrl_pipeline {PIPELINE_READY} {1}
    # connections and connection parameters - st pipeline, 
    # put it in front of the xip xip_controller
    add_connection clk_bridge.out_clk csr_ctrl_pipeline.cr0 clock
    add_connection reset_bridge.out_reset csr_ctrl_pipeline.cr0_reset reset
    # route the output of the xip controller to this pipeline stage
    add_connection csr_controller.cmd_pck csr_ctrl_pipeline.sink0 avalon_streaming
    add_connection csr_ctrl_pipeline.source0  multiplexer.sink1 avalon_streaming 
    # connections and connection parameters - csr_controller
    #add_connection csr_controller.cmd_pck multiplexer.sink0 avalon_streaming
    } else {
     add_connection csr_controller.cmd_pck multiplexer.sink1 avalon_streaming
        
    }

    if {$pipe_xip == 1} {
        add_instance xip_ctrl_pipeline altera_avalon_st_pipeline_stage 18.0
        set_instance_parameter_value xip_ctrl_pipeline {SYMBOLS_PER_BEAT} {1}
        set_instance_parameter_value xip_ctrl_pipeline {BITS_PER_SYMBOL} {32}
        set_instance_parameter_value xip_ctrl_pipeline {USE_PACKETS} {1}
        set_instance_parameter_value xip_ctrl_pipeline {USE_EMPTY} {0}
        set_instance_parameter_value xip_ctrl_pipeline {CHANNEL_WIDTH} {2}
        set_instance_parameter_value xip_ctrl_pipeline {MAX_CHANNEL} {0}
        set_instance_parameter_value xip_ctrl_pipeline {ERROR_WIDTH} {0}
        set_instance_parameter_value xip_ctrl_pipeline {PIPELINE_READY} {1}
        # connections and connection parameters - st pipeline, 
        # put it in front of the xip xip_controller
        add_connection clk_bridge.out_clk xip_ctrl_pipeline.cr0 clock
        add_connection reset_bridge.out_reset xip_ctrl_pipeline.cr0_reset reset
        # route the output of the xip controller to this pipeline stage
        add_connection xip_controller.cmd_pck xip_ctrl_pipeline.sink0 avalon_streaming
        add_connection xip_ctrl_pipeline.source0  multiplexer.sink0 avalon_streaming 
        # connections and connection parameters - csr_controller
        #add_connection csr_controller.cmd_pck multiplexer.sink0 avalon_streaming
    } else {
        add_connection xip_controller.cmd_pck multiplexer.sink0 avalon_streaming
    }


    
    add_connection clk_bridge.out_clk csr_controller.clk clock
    add_connection reset_bridge.out_reset csr_controller.reset reset
    add_connection merlin_demultiplexer_0.src1 csr_controller.rsp_pck avalon_streaming
    add_interface avl_csr avalon slave
    set_interface_property avl_csr EXPORT_OF csr_controller.csr

    # connections and connection parameters - xip_controller
    add_connection clk_bridge.out_clk xip_controller.clk clock
    add_connection reset_bridge.out_reset xip_controller.reset reset
    add_connection merlin_demultiplexer_0.src0 xip_controller.rsp_pck avalon_streaming
    add_interface avl_mem avalon slave
    set_interface_property avl_mem EXPORT_OF xip_controller.mem

    # connections and connection parameters - demux and mux
    add_connection clk_bridge.out_clk multiplexer.clk clock
    add_connection clk_bridge.out_clk merlin_demultiplexer_0.clk clock
    add_connection reset_bridge.out_reset multiplexer.clk_reset reset
    add_connection reset_bridge.out_reset merlin_demultiplexer_0.clk_reset reset

    # exported interfaces

    add_interface clk clock sink
    set_interface_property clk EXPORT_OF clk_bridge.in_clk
    add_interface reset reset sink
    set_interface_property reset EXPORT_OF reset_bridge.in_reset

    # In case ASMI 2, the chip select is from the CSR< user should use csr comnponent to select the devices
    # then XIP controller will talk to that device, 
    # In case QSPI 2, the want the XIP to have dedicated chip select, since they embeded the chip select into the rd_wr_byte_addressing
    # So base on this, set correct value chip select enbable for the two CSR and XIP
    if {$use_chip_sel_from_csr == 1} {
        set_instance_parameter_value csr_controller {CHIP_SELECT_EN} {1}
        set_instance_parameter_value xip_controller {CHIP_SELECT_EN} {1}
        # connect chip select from the CSR to XIP controller
        add_connection csr_controller.chip_select xip_controller.chip_select conduit
    } else {
        set_instance_parameter_value csr_controller {CHIP_SELECT_EN} {0}
        set_instance_parameter_value xip_controller {CHIP_SELECT_EN} {1}
        add_interface chip_select conduit end
        set_interface_property chip_select EXPORT_OF xip_controller.chip_select
    }

switch $debug_option {
    0 {
        add_instance asmi2_cmd_generator_0 altera_asmi2_cmd_generator 

        add_instance asmi2_qspi_interface_0 iptronix_asmi2_qspi_interface 
        set_instance_parameter_value asmi2_qspi_interface_0 {DATA_WIDTH} $io_mode
        set_instance_parameter_value asmi2_qspi_interface_0 {DISABLE_ASMIBLOCK} $disable_asmiblock
        set_instance_parameter_value asmi2_qspi_interface_0 {USE_GPIO} $use_gpio
        #set_instance_parameter_value asmi2_qspi_interface_0 {NCS_NUM} $user_ncs_num
        set_instance_parameter_value asmi2_qspi_interface_0 {NCS_NUM} $chip_sels
        set_instance_parameter_value asmi2_qspi_interface_0 {ENABLE_SIM} $enable_sim_model
         
        if { $use_gpio } {
            add_interface qspi_pins conduit end
            set_interface_property qspi_pins EXPORT_OF asmi2_qspi_interface_0.qspi_pins
        } elseif { $disable_asmiblock } {
            add_interface atom_ports conduit end
            set_interface_property atom_ports EXPORT_OF asmi2_qspi_interface_0.atom_ports
        }

        add_connection csr_controller.addr_bytes_csr asmi2_cmd_generator_0.addr_bytes_csr conduit
        add_connection asmi2_cmd_generator_0.addr_bytes_xip xip_controller.addr_bytes_xip conduit

        add_connection clk_bridge.out_clk asmi2_cmd_generator_0.clk clock
        add_connection reset_bridge.out_reset asmi2_cmd_generator_0.reset reset
        add_connection asmi2_cmd_generator_0.out_rsp_pck merlin_demultiplexer_0.sink avalon_streaming
        #add_connection multiplexer.src asmi2_cmd_generator_0.in_cmd_pck avalon_streaming
        add_connection clk_bridge.out_clk asmi2_qspi_interface_0.clk clock
        add_connection reset_bridge.out_reset asmi2_qspi_interface_0.reset reset
        
        #add_connection asmi2_cmd_generator_0.out_cmd_pck asmi2_qspi_interface_0.in_cmd_pck avalon_streaming
        add_connection asmi2_qspi_interface_0.out_rsp_pck asmi2_cmd_generator_0.in_rsp_pck avalon_streaming

        add_connection asmi2_cmd_generator_0.dummy_cycles asmi2_qspi_interface_0.dummy_cycles conduit
        add_connection asmi2_cmd_generator_0.chip_select asmi2_qspi_interface_0.chip_select conduit
        add_connection asmi2_cmd_generator_0.require_rdata asmi2_qspi_interface_0.require_rdata conduit
        add_connection csr_controller.qspi_interface_en asmi2_qspi_interface_0.qspi_interface_en conduit
        #add_interface rsp avalon_streaming sink
        #set_interface_property rsp EXPORT_OF asmi2_cmd_generator_0.in_rsp_pck
        #add_interface cmd avalon_streaming source
        #set_interface_property cmd EXPORT_OF asmi2_cmd_generator_0.out_cmd_pck

        # Need add in SPI interfac component
        if {$pipe_cmd_gen_cmd ==1} {
            add_instance cmd_gen_pipe_cmd altera_avalon_st_pipeline_stage 18.0
            set_instance_parameter_value cmd_gen_pipe_cmd {SYMBOLS_PER_BEAT} {1}
            set_instance_parameter_value cmd_gen_pipe_cmd {BITS_PER_SYMBOL} {8}
            set_instance_parameter_value cmd_gen_pipe_cmd {USE_PACKETS} {1}
            set_instance_parameter_value cmd_gen_pipe_cmd {USE_EMPTY} {0}
            set_instance_parameter_value cmd_gen_pipe_cmd {CHANNEL_WIDTH} {2}
            set_instance_parameter_value cmd_gen_pipe_cmd {MAX_CHANNEL} {0}
            set_instance_parameter_value cmd_gen_pipe_cmd {ERROR_WIDTH} {0}
            set_instance_parameter_value cmd_gen_pipe_cmd {PIPELINE_READY} {1}
            
            add_connection clk_bridge.out_clk cmd_gen_pipe_cmd.cr0 clock
            add_connection reset_bridge.out_reset cmd_gen_pipe_cmd.cr0_reset reset
            add_connection asmi2_cmd_generator_0.out_cmd_pck cmd_gen_pipe_cmd.sink0 avalon_streaming
            add_connection cmd_gen_pipe_cmd.source0 asmi2_qspi_interface_0.in_cmd_pck avalon_streaming

        } else {
            add_connection asmi2_cmd_generator_0.out_cmd_pck asmi2_qspi_interface_0.in_cmd_pck avalon_streaming
        }

        # pipeline output of the mux
        if {$pipe_mux_cmd ==1} {
            add_instance mux_output_pipe altera_avalon_st_pipeline_stage 18.0
            set_instance_parameter_value mux_output_pipe {SYMBOLS_PER_BEAT} {1}
            set_instance_parameter_value mux_output_pipe {BITS_PER_SYMBOL} {32}
            set_instance_parameter_value mux_output_pipe {USE_PACKETS} {1}
            set_instance_parameter_value mux_output_pipe {USE_EMPTY} {0}
            set_instance_parameter_value mux_output_pipe {CHANNEL_WIDTH} {2}
            set_instance_parameter_value mux_output_pipe {MAX_CHANNEL} {0}
            set_instance_parameter_value mux_output_pipe {ERROR_WIDTH} {0}
            set_instance_parameter_value mux_output_pipe {PIPELINE_READY} {1}
            
            add_connection clk_bridge.out_clk mux_output_pipe.cr0 clock
            add_connection reset_bridge.out_reset mux_output_pipe.cr0_reset reset
            add_connection multiplexer.src mux_output_pipe.sink0 avalon_streaming
            add_connection mux_output_pipe.source0 asmi2_cmd_generator_0.in_cmd_pck avalon_streaming

        } else {
            add_connection multiplexer.src asmi2_cmd_generator_0.in_cmd_pck avalon_streaming
        }

    }

    1 {
        add_interface addr_bytes_csr conduit end
        set_interface_property addr_bytes_csr EXPORT_OF csr_controller.addr_bytes_csr
        add_interface addr_bytes_xip conduit end
        set_interface_property addr_bytes_xip EXPORT_OF xip_controller.addr_bytes_xip
        add_interface rsp avalon_streaming sink
        set_interface_property rsp EXPORT_OF merlin_demultiplexer_0.sink
        add_interface cmd avalon_streaming source
        set_interface_property cmd EXPORT_OF multiplexer.src
        add_interface qspi_interface_en conduit end
        set_interface_property qspi_interface_en EXPORT_OF csr_controller.qspi_interface_en
    }
    2 {
        add_instance asmi2_cmd_generator_0 altera_asmi2_cmd_generator 

        add_connection csr_controller.addr_bytes_csr asmi2_cmd_generator_0.addr_bytes_csr conduit
        add_connection asmi2_cmd_generator_0.addr_bytes_xip xip_controller.addr_bytes_xip conduit

        add_connection clk_bridge.out_clk asmi2_cmd_generator_0.clk clock
        add_connection reset_bridge.out_reset asmi2_cmd_generator_0.reset reset
        add_connection asmi2_cmd_generator_0.out_rsp_pck merlin_demultiplexer_0.sink avalon_streaming
        add_connection multiplexer.src asmi2_cmd_generator_0.in_cmd_pck avalon_streaming
        add_interface rsp avalon_streaming sink
        set_interface_property rsp EXPORT_OF asmi2_cmd_generator_0.in_rsp_pck
        add_interface cmd avalon_streaming source
        set_interface_property cmd EXPORT_OF asmi2_cmd_generator_0.out_cmd_pck
        add_interface qspi_interface_en conduit end
        set_interface_property qspi_interface_en EXPORT_OF csr_controller.qspi_interface_en
        add_interface dummy_cycles conduit end
        set_interface_property dummy_cycles EXPORT_OF asmi2_cmd_generator_0.dummy_cycles
        add_interface require_rdata conduit end
        set_interface_property require_rdata EXPORT_OF asmi2_cmd_generator_0.require_rdata
        add_interface chip_select conduit end
        set_interface_property chip_select EXPORT_OF asmi2_cmd_generator_0.chip_select
    }
}
}
