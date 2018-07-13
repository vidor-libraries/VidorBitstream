#include "VidorLib.h"
#include "VidorI2C.h"
#include "VidorSPI.h"
#include "VidorUART.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }
  Serial.println(FPGA.read(0), HEX);
  Serial.println(FPGA.read(1), HEX);
  Serial.println(FPGA.read(2), HEX);
  Serial.println(FPGA.read(3), HEX);
  Serial.println(FPGA.read(4), HEX);

  pinMode(105, OUTPUT);
  digitalWrite(105, HIGH);

  WireEx.begin();

  SPIEx.begin();
  SPIEx.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  SPIEx.transfer(12);
  SPIEx.endTransaction();

  SerialEx.begin(115200);
  while (!SerialEx);
  SerialEx.println("test");

  FPGA.end();
}

void loop() {
  // put your main code here, to run repeatedly:

}
