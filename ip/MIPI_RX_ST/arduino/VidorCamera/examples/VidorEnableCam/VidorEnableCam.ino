/*
  This sketch shows how to control a camera on a MKRVidor4000.
  The camera will start streaming to HDMI output as soon as the FPGA bitstream is loaded.
  Type "STOP" on the serial monitor to pause the streaming and "START" to restart.

  Circuit:
  - MKR Vidor 4000;
  - Raspberry Pi Camera V1 (OV5647) connected to strip header
  - microHDMI to HDMI cable
  - an HDMI capable display

  created 25 July 2018
  by Riccardo Rizzo
*/

#_INCLUDE_PARENT_LIB_
#include "VidorCamera.h"

VidorCamera vcam;

void setup() {
  Serial.begin(9600);
  
  // wait for the serial monitor to open,
  // if you are powering the board from a USB charger remove the next line
  while (!Serial) {}

  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }

  // begin() enables the I2C communication and initializes the camera

  if (!vcam.begin()) {
    Serial.println("Camera begin failed");
    while (1) {}
  }

  delay(4000);
  Serial.println("Power On");

  // The camera should be on now, streaming to the HDMI output
}

void loop()
{
  // Wait for the user input to stop the camera
  String res = Serial.readStringUntil('\n');
  if (res.indexOf("STOP") > 0) {
    vcam.end();
  }
  if (res.indexOf("START") > 0) {
    vcam.begin();
  }
}
