Copy files:

- boot.ttf to vidor_s_boot.ttf
- vidor_s.ttf, boot.svf and nios_app.elf without changing name

# Convert NIOS2 elf program to flashable image
launch ./make_app.sh

./createCompositeBinary -ttf=../vidor_s.ttf -bin=../nios_app.bin -invert -out=../app.
./createCompositeBinary -ttf=../vidor_s_boot.ttf -bin=../nios_app.bin -invert -out=../boot.

# Load bitstream to ram
./loadBitsreamToRam -openocd=.../.arduino15/packages/arduino/tools/openocd/0.10.0-arduino7/ -port=/dev/ttyACM1 -svf=../boot.svf

#load boot bitsream to flash
-> add #include "VidorBoot.h" to a sketch and upload

#load app bitsream to flash
-> add #include "VidorApp.h" to a sketch and upload
