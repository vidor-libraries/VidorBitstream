# Convert NIOS2 elf program to flashable image
```
./make_app.sh
```

# To create bootloader + app package
```
./createCompositeBinary -i ../vidor_s_boot.ttf:1:0,../vidor_s.ttf:1:512 -o ../boot.ttf -t 0
```

# To create app + nios_app package
```
./createCompositeBinary -i ../vidor_s.ttf:1:512,../nios_app.bin:0:896: -o ../app.ttf -t 1
```

# Directly load bitstream to ram
```
flash clujtag-server on the MKRVidor
./clujtag-client -p PORTNAME -s SVF_FILENAME
```
or
```
flash USBBlaster library example
use Quartus Programmer or urjtag to interact with the FPGA
```
