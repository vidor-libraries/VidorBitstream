# Utility to create composite binary for MKR Vidor 4000

Bitstreams produced by Quartus (in ttf format) are not suitable to be directly burned on the flash since their nibble encoding is reverse.
This utility helps creating the correct file format, appending other sections if needed.

The accepted parameters are:

`-i` list all the input files with some metadata, comma separated; 

  - the first parameter after the filename represents the file type (0 -> raw binary, 1 -> ttf from Quartus)
  - the second parameter represents the absolute offset in the resulting image, expressed in KB
  
 `-o` specifies the output file
 
 `-t` speficies the image type ( 0 -> boot image, 1 -> app image, 2 -> user data)
 
 Example usage:
 
 `./createCompositeBinary -i ../vidor_s.ttf:1:512,../nios_app.bin:0:896 -o ../app.ttf -t 1`
 
 Creates a flashable file called `app.ttf` composed by `vidor_s.ttf` (from Quartus, to be flashed at 512KB) and `/nios_app.bin` (raw binary, to be flashed at 896KB).
 The resulting image is an app image.
 The output will resemble
 
```
NO_BOOTLOADER,
0x00, 0x00, 0x08, 0x00,
0x89, 0xf3, 0x11, 0x00,
0x9f, 0xa1, 0x70, 0xa8, 0x70, 0xdc, 0x3d, 0x08, 0x18, 0xe4, 0xdf, 0xb1, 0xac, 0x7a, 0x8f, 0x67, 0x88, 0xea, 0xfa, 0x09, 0x38, 0x43, 0x52, 0x80, 0x04, 0xce, 0xec, 0x50, 0x48, 0x44, 0x5e, 0x07, 
0x01, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, // force flag, change to 1 if needed
NO_USER_DATA,
```

This snippet need to be copied to the .fpga_bitstream_signature section in your main cpp file (see https://github.com/bcmi-labs/VidorLibs/blob/master/VidorGraphics/src/VidorGraphics.cpp#L6 for reference)
