# BSP options
SOPC_CPU_NAME="nios2_gen2_0"
SOPC_CODE_MEMORY_NAME="onchip_memory2_0"

SIMULATION_OPTIMIZED_SUPPORT="false"
BSP_TYPE=hal
BSP_FLAGS=" \
--cmd enable_sw_package CFG \
--cmd enable_sw_package SF \
--cmd enable_sw_package AES \
--cmd enable_sw_package SIGN \
--cmd enable_sw_package MAILBOX \
--cmd enable_sw_package RPC \
--cmd enable_sw_package GFX \
--cmd enable_sw_package QRCODE_FINDER \
--cmd enable_sw_package TMR \
--set hal.make.bsp_cflags_defined_symbols -DNO_RPC=1 \
--set hal.enable_c_plus_plus 0 \
--set hal.enable_clean_exit 0 \
--set hal.enable_exit 0 \
--set hal.enable_gprof 0 \
--set hal.enable_lightweight_device_driver_api 1 \
--set hal.enable_mul_div_emulation 0 \
--set hal.enable_reduced_device_drivers 1 \
--set hal.enable_runtime_stack_checking 0 \
--set hal.enable_sim_optimize 0 \
--set hal.enable_small_c_library 1 \
--set hal.enable_sopc_sysid_check 1 \
--set hal.enable_sim_optimize $SIMULATION_OPTIMIZED_SUPPORT \
--set hal.make.bsp_cflags_optimization $OPTIMIZATION_LEVEL \
--set hal.linker.allow_code_at_reset 1 \
--set hal.linker.enable_alt_load 0 \
--set hal.linker.enable_alt_load_copy_exceptions 0 \
--set hal.linker.enable_alt_load_copy_rodata 0 \
--set hal.linker.enable_alt_load_copy_rwdata 1 \
--set hal.linker.enable_exception_stack 0 \
--set hal.linker.enable_interrupt_stack 0 \
--set hal.linker.exception_stack_memory_region_name $SOPC_CODE_MEMORY_NAME \
--set hal.linker.interrupt_stack_memory_region_name $SOPC_CODE_MEMORY_NAME \
--set hal.make.ignore_system_derived.big_endian 0 \
--set hal.make.ignore_system_derived.debug_core_present 0 \
--set hal.make.ignore_system_derived.fpu_present 0 \
--set hal.make.ignore_system_derived.hardware_divide_present 0 \
--set hal.make.ignore_system_derived.hardware_fp_cust_inst_divider_present 0 \
--set hal.make.ignore_system_derived.hardware_fp_cust_inst_no_divider_present 0 \
--set hal.make.ignore_system_derived.hardware_multiplier_present 0 \
--set hal.make.ignore_system_derived.hardware_mulx_present 0 \
--set hal.make.ignore_system_derived.sopc_simulation_enabled 0 \
--set hal.make.ignore_system_derived.sopc_system_base_address 0 \
--set hal.make.ignore_system_derived.sopc_system_id 0 \
--set hal.make.ignore_system_derived.sopc_system_timestamp 0 \
--set hal.max_file_descriptors 4 \
--set hal.stderr none \
--set hal.stdin none \
--set hal.stdout none \
--set hal.sys_clk_timer none \
--script scripts/set_regions.tcl \
--cmd set_driver arduino_generic_quad_spi_controller2 qspi \
--cmd set_driver none nina_uart \
--cmd add_section_mapping .rwdata onchip_memory2_0 \
--cmd add_section_mapping .bss onchip_memory2_0 \
--cmd add_section_mapping .heap onchip_memory2_0 \
--cmd add_section_mapping .entry onchip_memory2_0 \
--cmd add_section_mapping .text onchip_memory2_0 \
--cmd add_section_mapping .rodata onchip_memory2_0 \
--cmd add_section_mapping .data onchip_memory2_0 \
--cmd add_section_mapping .stack onchip_memory2_0 \
"

if [ "x"$LITE == "x" ]; then
EXTRA_FLAGS=" \
--set altera_vic_driver.linker_section .rwdata \
--cmd enable_sw_package UART \
"
else
EXTRA_FLAGS="\
--set hal.make.bsp_cflags_user_flags \
-DFREE_VERSION=1 \
"
fi