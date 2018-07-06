#!/bin/bash

LIB_NAME=$1
BASE_FOLDER=`dirname $0`

cd $BASE_FOLDER/../

declare -a libs=("FPGA_GFX_COUNT" "FPGA_SPI_INTERFACES_COUNT" "FPGA_I2C_INTERFACES_COUNT" "FPGA_UART_INTERFACES_COUNT" "FPGA_ENCODERS_COUNT" "FPGA_NEOPIXEL_COUNT" "FPGA_CAMERA_COUNT" )
declare -a folders=("Vidor_GFX" "VidorSPI" "VidorI2C" "VidorUART" "VidorEncoder" "VidorNeopixel" "VidorCamera")


mkdir -p ./distrib/$LIB_NAME/

cp -r $LIB_NAME/* ./distrib/$LIB_NAME/

# get length of an array
arraylength=${#libs[@]}

for (( i=1; i<${arraylength}; i++ ));
do
HAS_LIB=`cat $LIB_NAME/src/defines.h | grep ${libs[i]}  | expand | tr -s ' ' | cut -f3 -d' ' `

echo ${folders[i]} $HAS_LIB

if [ x$HAS_LIB != x ] && [ $HAS_LIB != 0 ]
then
	cp ${folders[i]}/* ./distrib/$LIB_NAME/src/
fi

done

