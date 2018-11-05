/*
   This sketch shows how to use the Encoder IP in MKRVidor 4000
   Quadrature encoders are a very common way to detect position (and speed) of a moving part, like a DC motor.
   Normally, in the microcontroller world, decoding a quadrature encoder has serious limitations since all egdes must be counted, 
   otherwise the final number will be wrong. 
   This is usually accomplished using interrupts, but over a certain revolution speed 
   the intinsic overhead of servicing an interrupt destroys the count reliability.

   Using the FPGA to perform decoding allows:
    - not to lose any edge until the revolution speed is less than some million RPM :)
    - we can "read" the encoder at any time, because the FPGA is counting independently

    Circuit: 
      connect ENC_A to A0 and ENC_B to A1
*/

#_INCLUDE_PARENT_LIB_
#include "VidorEncoder.h"

// Initialize Encoder connected to A0 and A1.
VidorEncoder encoder(A0, A1);

void setup() {
  Serial.begin(9600);

  // wait for the serial monitor to open,
  // if you are powering the board from a USB charger remove the next line
  while (!Serial);

  // Let's start the FPGA
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }
}

void loop() {

  // Read the encoder
  int value = encoder.read();
  Serial.print("Encoder value: ");
  Serial.println(value);

#if 1

  // Wait one second with interrupts disabled
  noInterrupts();
  // We can't call delay() since it uses interrupts, so use busy loops
  for (int i = 0; i < F_CPU / 10; i++) {
    asm ("nop");
  }
  interrupts();

#else
  delay(200);
#endif

  if (value >= 200 || value <= -200) {
    // Reset the count
    encoder.write(0);
  }
}
