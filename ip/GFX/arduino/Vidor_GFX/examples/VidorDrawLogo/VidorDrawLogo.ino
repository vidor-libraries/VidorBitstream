#_INCLUDE_PARENT_LIB_
#include "Vidor_GFX.h"

Vidor_GFX  vdgfx;

void setup() {
  Serial.begin(9600);

  // Initialize the FPGA
  if (!FPGA.begin()) {
    Serial.println("Initialization failed!");
    while (1) {}
  }

  vdgfx.begin();
}

void loop()
{
  /**
  *  Draw an Arduino logo
  */

  // Fill the screen with a white background
  vdgfx.fillRect(0,0,640,480,vdgfx.White());

  /**
  *  The library allows drawing some basic elements to the view, like circles, rectangles, lines
  */  

  vdgfx.fillCircle(225,225,100 ,vdgfx.lightBlue());
  vdgfx.fillCircle(415,225,100 ,vdgfx.lightBlue());
  vdgfx.fillCircle(225,225,90 ,vdgfx.White());
  vdgfx.fillCircle(415,225,90 ,vdgfx.White());
  vdgfx.fillRect(175,220,100,10 ,vdgfx.lightBlue());
  vdgfx.fillRect(365,220,100,10 ,vdgfx.lightBlue());
  vdgfx.fillRect(410,175,10,100 ,vdgfx.lightBlue());

  /**
  *  To draw a text we can use the classic functions like write() and print()
  *  Text size, color and position can be changed using the .text subclass
  */
  vdgfx.text.setFont(0);
  vdgfx.text.setCursor(150,375);
  vdgfx.text.setAlpha(255);
  vdgfx.text.setSize(3);
  vdgfx.text.setColor(vdgfx.lightBlue());
  vdgfx.println("ARDUINO");
  vdgfx.text.setCursor(480,145);
  vdgfx.text.setSize(1);
  vdgfx.println("TM");

  while (1) {
  }
}
