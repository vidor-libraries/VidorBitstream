# Get region information for onchip_ram memory region.
# Returned as a list.
set region_info [get_memory_region iptronix_generic_quad_spi_controller2_0_avl_mem]
# Extract fields from region information list.
set region_name [lindex $region_info 0]
set slave_desc [lindex $region_info 1]
set offset [lindex $region_info 2]
set span [lindex $region_info 3]
# Remove the existing memory region.
delete_memory_region $region_name
# Compute memory ranges for replacement regions.
set split_span [expr (1024+128)*1024]
set new_span [expr $span-$split_span]
set split_offset [expr $offset+$new_span]
# Create two memory regions out of the original region.
add_memory_region reserved $slave_desc $offset $new_span
add_memory_region iptronix_generic_quad_spi_controller2_0_avl_mem $slave_desc $split_offset $split_span
delete_section_mapping .data
