QUARTUS_FOLDER=/ssd/intelFPGA_lite/18.0/
export QUARTUS_ROOTDIR=$QUARTUS_FOLDER/quartus

$QUARTUS_FOLDER/nios2eds/bin/elf2flash --input=nios_app.elf --output=nios_app.flash --epcs --verbose
$QUARTUS_FOLDER/nios2eds/bin/gnu/H-x86_64-pc-linux-gnu/bin/nios2-elf-objcopy --input-target srec --output-target binary nios_app.flash nios_app.bin
#xxd -i nios_app.bin | sed  '1d;$d' | sed '$d' > nios_app.ttf
#now grab bitstream and pad next section to 384KB

