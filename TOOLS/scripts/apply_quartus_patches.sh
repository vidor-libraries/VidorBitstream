BASEDIR="`dirname \"$0\"`"              # relative
BASEDIR="`( cd \"$BASEDIR\" && pwd )`"  # absolutized and normalized
ORIGINAL_VERSION="18.0"
ACTUAL_VERSION=`grep -Po '\d{2}\.\d{1}'  <<< $QUARTUS_ROOTDIR`
echo PATCH_VERSION ${ORIGINAL_VERSION} ACTUAL_VERSION ${ACTUAL_VERSION}
if [ "${_IS_WINDOWS}" = "1" ] && [ -n "$(which cygpath 2>/dev/null)" ]; then
  QUARTUS_ROOTDIR="$(cygpath -u "${QUARTUS_ROOTDIR}" 2>/dev/null)"
  QUARTUS_IPDIR="$(cygpath -u "${QUARTUS_ROOTDIR}" 2>/dev/null)"/../ip/altera
else
  QUARTUS_IPDIR="${QUARTUS_ROOTDIR}"/../ip/altera
fi
sed -i -- "s/${ORIGINAL_VERSION}/${ACTUAL_VERSION}/g" patches/*
cd $QUARTUS_IPDIR
rm -rf pgm/arduino_generic_qspi_controller2
mkdir pgm/arduino_generic_qspi_controller2
cp -r  pgm/generic_qspi_controller2/* pgm/arduino_generic_qspi_controller2
mv pgm/arduino_generic_qspi_controller2/altera_generic_quad_spi_controller2_sw.tcl pgm/arduino_generic_qspi_controller2/arduino_generic_quad_spi_controller2_sw.tcl
mv pgm/arduino_generic_qspi_controller2/generic_qspi_controller2_hw.tcl pgm/arduino_generic_qspi_controller2/arduino_generic_qspi_controller2_hw.tcl
mv pgm/arduino_generic_qspi_controller2/HAL/src/altera_generic_quad_spi_controller2.c pgm/arduino_generic_qspi_controller2/HAL/src/arduino_generic_quad_spi_controller2.c
mv pgm/arduino_generic_qspi_controller2/HAL/inc/altera_generic_quad_spi_controller2.h pgm/arduino_generic_qspi_controller2/HAL/inc/arduino_generic_quad_spi_controller2.h
mv pgm/arduino_generic_qspi_controller2/inc/altera_generic_quad_spi_controller2_regs.h pgm/arduino_generic_qspi_controller2/inc/arduino_generic_quad_spi_controller2_regs.h
cp pgm/altera_asmi_parallel2/qspi_interface/altera_asmi2_qspi_interface_gpio.sv pgm/arduino_generic_qspi_controller2/altera_asmi2_qspi_interface_gpio.sv
cp pgm/altera_asmi_parallel2/qspi_interface/altera_asmi2_qspi_interface.sv.terp pgm/arduino_generic_qspi_controller2/arduino_asmi2_qspi_interface.sv.terp
cp pgm/altera_asmi_parallel2/qspi_interface/altera_asmi2_qspi_interface_hw.tcl pgm/arduino_generic_qspi_controller2/arduino_asmi2_qspi_interface_hw.tcl
cp pgm/altera_asmi_parallel2/asmi_top/altera_asmi_parallel2_hw.tcl pgm/arduino_generic_qspi_controller2/arduino_asmi_parallel2_hw.tcl
cp pgm/altera_asmi_parallel2/asmi_top/altera_asmi_parallel2_hw_proc.tcl pgm/arduino_generic_qspi_controller2/arduino_asmi_parallel2_hw_proc.tcl
cp pgm/altera_asmi_parallel2_top/altera_asmi_parallel2_top_hw_proc.tcl pgm/arduino_generic_qspi_controller2/arduino_asmi_parallel2_top_hw_proc.tcl
cp pgm/altera_asmi_parallel2/presets/N25Q016.qprs pgm/arduino_generic_qspi_controller2/N25Q016.qprs
cp pgm/altera_asmi_parallel2/asmi_top/altera_asmi_parallel2_ui_settings.tcl pgm/arduino_generic_qspi_controller2
cp pgm/altera_asmi_parallel2/asmi_top/altera_asmi_parallel2_ui_configuration.tcl pgm/arduino_generic_qspi_controller2
find pgm/arduino_generic_qspi_controller2/* -type f -print0 | xargs -0 dos2unix pgm/arduino_generic_qspi_controller2/*
patch -p0 --ignore-whitespace < $BASEDIR/patches/qspi.patch
rm -rf arduino_16550_uart
cp -r altera_16550_uart/ arduino_16550_uart
mv arduino_16550_uart/altera_16550_uart_hw.tcl  arduino_16550_uart/arduino_16550_uart_hw.tcl
mv arduino_16550_uart/altera_16550_uart_sw.tcl  arduino_16550_uart/arduino_16550_uart_sw.tcl
for f in arduino_16550_uart/HAL/src/*.c; do mv "$f" "$(echo "$f" | sed s/altera/arduino/)"; done
for f in arduino_16550_uart/HAL/inc/*.h; do mv "$f" "$(echo "$f" | sed s/altera/arduino/)"; done
mv arduino_16550_uart/inc/altera_16550_uart_regs.h  arduino_16550_uart/inc/arduino_16550_uart_regs.h
mv arduino_16550_uart/altera_16550_uart.vc arduino_16550_uart/arduino_16550_uart.vc
find arduino_16550_uart/* -type f -print0 | xargs -0 dos2unix arduino_16550_uart/*
patch -p0 --ignore-whitespace < $BASEDIR/patches/uart.patch
cd $BASEDIR