/*
  This sketch shows how to use the NeoPixel library in MKRVidor 4000
  It draws on a 16x16 neopixel panel using gfx library
  In this example all the commands are sent to pin A6 to the panel.
  Circuit:
  - MKR Vidor 4000;
  - External Power Supply
  - Neopixel panel:
    - Neopixel GND connected to  MKRVIDOR4000 GND and Power supply V-;
    - Power supply V+ connected to 5V+ Neopixel;
    - pin A6 connected to the Neopixel control Pin D0.
  created 12 August 2018
  by Dario Pennisi
*/

#_INCLUDE_PARENT_LIB_
#include "Vidor_NeoPixel.h"

Vidor_NeoPixel    np(256, A6); // The constructor allows you to set the control pin and the number of the LED's.
Vidor_GFX         vdgfx(np);
Vidor_GFXbuffer   matrix(np, vdgfx, 128, 16, true);  // Specify a buffer that will be used by the NeoPixel core (think about it as a canvas)

int count = 0;

void setup() {

  Serial.begin(115200);
  while (!Serial) {}
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }

  Serial.println("FPGA Initialization Done!");
  Serial.println(FPGA.version(), HEX);

  FPGA.printConfig();
  Serial.println("Initialize NeoPixels");

  np.begin();
  matrix.begin();

  for (int i = 0; i < 256; i++) {
    // Clear matrix
    np.setPixelColor(i, 0, 0, 0, 0);
  }
  np.show();
}

void loop()  {

  // Wait for user input to start/stop the scroll
  String s = "";
  while (Serial.available()) {
    s += (char)Serial.read();
  }
  npTest(s);
}
void npTest(String action)
{
  action.toLowerCase();
  action.trim();

  if (action == "draw") {
    //Draw a text
    vdgfx.text.setColor(vdgfx.Red());
    vdgfx.text.setCursor(0, 6);
    vdgfx.setFont(0);
    vdgfx.text.setSize(1);
    vdgfx.print("HELLO WORLD!");

    np.show();
  }

  if (action == "scroll") {
    // Scroll one pixel every 1000 milliseconds
    matrix.scroll(1000);
  }

  if (action == "stop") {
    matrix.noScroll();
  }

  if (action == "paint") {
    vdgfx.drawRect(0, 0, 6, 6, 0x00FFFFFF, 0xFF);
    np.show();
  }
}
