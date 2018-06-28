#include "VidorUART.h"
#include "VIDOR_CAM.h"

Vidor_CAM vcam;


void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  if (!VD.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }
  /**
  *     begin() enable the I2C communication and initialize the display for the camera
  */
  if(vcam.begin()){
    Serial.println("Camera begin failed");
    while (1) {}
  }
  /**
  *     setPower() turn on the camera
  */
  if(!vcam.setPower(true)){
    Serial.println("Power On failed");
    while (1) {}
  }

  delay(4000);
      Serial.println("Power On ok");
}

void loop()
{

}
