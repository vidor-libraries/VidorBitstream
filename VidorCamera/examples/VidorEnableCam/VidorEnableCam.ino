#include "VidorGraphics.h"
#include "VidorCamera.h"

VidorCamera vcam;

void setup() {

  Serial.begin(115200);
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
}
