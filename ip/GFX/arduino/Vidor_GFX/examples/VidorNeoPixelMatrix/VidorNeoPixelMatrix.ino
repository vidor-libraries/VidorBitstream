/*
  This sketch shows how to use the NeoPixel library in MKRVidor 4000
  It draws on a 16x16 neopixel panel using gfx library
  In this example all the commands are sent to pin A6 to the panel.
  To send the commands open the serial port and wait for instructions.
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

Vidor_NeoPixel    np(16*16, A6); // The constructor allows you to set the control pin and the number of the LED's.
Vidor_GFX         vdgfx(np);

// Specify a buffer that will be used by the NeoPixel core
// To initialize a Neopixel matrix specify x and y dimensions of the virtual canvas (128 and 16 in this case) and the strip arrangement (true if zigzag)
Vidor_GFXbuffer   matrix(vdgfx, 128, 16, true);

void setup() {

  Serial.begin(9600);
  while (!Serial) {}
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }

  matrix.begin();

  for (int i = 0; i < 256; i++) {
    // Clear matrix
    np.setPixelColor(i, 0, 0, 0, 0);
  }
  np.show();

  Serial.println("Available commands:");
  Serial.println("* \"draw\" : displays a string");
  Serial.println("* \"scroll\" : start scrolling the virtual canvas");
  Serial.println("* \"stop\" : stop scrolling");
  Serial.println("* \"paint\" : paint a rectangle using GFX APIs");
}

void loop()  {

  // Wait for user input to start/stop the scroll
  String s = "";
  while (Serial.available()) {
    s += (char)Serial.read();
  }
  executeAction(s);
}

void executeAction(String action)
{
  action.toLowerCase();
  action.trim();

  if (action == "draw") {
    //Draw a text
    vdgfx.text.setColor(vdgfx.Red());
    vdgfx.text.setCursor(0, 6);
    vdgfx.text.setFont(0);
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
