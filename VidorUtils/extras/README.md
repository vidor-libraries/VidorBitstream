Copy files:

- boot.ttf to vidor_s_boot.ttf
- vidor_s.ttf, boot.svf and nios_app.elf without changing name

# Convert NIOS2 elf program to flashable image
launch ./make_app.sh

#To create bootloader + app package

./createCompositeBinary -i ../vidor_s_boot.ttf:1:0,../vidor_s.ttf:1:512 -o ../boot.ttf -t 0

# To create app + nios_app package
./createCompositeBinary -i ../vidor_s.ttf:1:512,../nios_app.bin:0:896: -o ../app.ttf -t 1

# Load bitstream to ram
./loadBitsreamToRam -openocd=.../.arduino15/packages/arduino/tools/openocd/0.10.0-arduino7/ -port=/dev/ttyACM1 -svf=../boot.svf

#load boot bitsream to flash
-> add #include "VidorBoot.h" to a sketch and upload

#load app bitsream to flash
-> add #include "VidorApp.h" to a sketch and upload
