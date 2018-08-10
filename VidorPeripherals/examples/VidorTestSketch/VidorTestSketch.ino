/*
   This sketch shows how to interact with the "standard" Vidor bitstream.
   A bitstream is like a program, but rather than containing code it describes how the FPGA should behave.
   Every Vidor bitstream (and library) provides a common set of functionalities
   Moreover, the library also includes all the "building block" as standard Arduino libraries
*/

#include "VidorPeripherals.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  // Let's start by initializing the FPGA
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }

  // Let's discover which version we are running
  int version = FPGA.version();
  Serial.print("Vidor bitstream version: ");
  Serial.println(version, HEX);

  // Let's also ask which IPs are included in this bitstream
  FPGA.printConfig();

  // Ok, so we know now that the FPGA contains the extended GPIO IP
  // Please refer to the online documentation for the actual pin assignment
  // Let's configure pin A0 to be an output, controlled by the FPGA
  FPGA.pinMode(33, OUTPUT);
  FPGA.digitalWrite(33, HIGH);

  // The same pin can be read by the SAMD processor :)
  pinMode(A0, INPUT);
  Serial.print("Pin A0 is ");
  Serial.println(digitalRead(A0) == LOW ? "LOW" : "HIGH");

  FPGA.digitalWrite(33, LOW);
  Serial.print("Pin A0 is ");
  Serial.println(digitalRead(A0) == LOW ? "LOW" : "HIGH");

  // The VidorBase bitstream provides various extra peripherals, that can be controlled with the standard Arduino API
  // To find which pins you need to use, please refer to the onlune documentation

  // WireFPGA1 object provides an extra I2C interface
  WireFPGA1.begin();

  // Same for SPIFPGA1, but with SPI of course
  SPIFPGA1.begin();
  SPIFPGA1.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  SPIFPGA1.transfer(12);
  SPIFPGA1.endTransaction();

  // Same for Serial interfaces :)
  SerialFPGA1.begin(115200);
  while (!SerialFPGA1);
  SerialFPGA1.println("test");

  // Ok, we are done, let's shutdown the FPGA (to save power, for example)
  FPGA.end();
}

void loop() {
  // put your main code here, to run repeatedly:

}
