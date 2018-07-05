#include "VIDOR_GFX.h"
Vidor_GFX  vdgfx;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial){
    
   }

  if (!VD.begin()) {
     Serial.println("Initialization failed!");
    while (1) {}
  }

  delay(4000);
}

void loop()
{
  /**
  *  Draw Arduino logo
  */
  vdgfx.fillRect(0,0,640,480,vdgfx.White(),1);
    /**
  *  The library VIDOR_GFX.h allow to draw some basic elements to the view, like circle rectangular line and point, simply it could be drawed by using the function: 
  *  - drawPixel()
  *  - drawLine()
  *  - drawRect()  
  *  - fillRect()
  *  - drawCircle()
  *  - fillCircle()
  *  - drawChar()
  */  
  vdgfx.fillCircle(225,225,100 ,vdgfx.lightBlue(),1);
  vdgfx.fillCircle(415,225,100 ,vdgfx.lightBlue(),1);
  vdgfx.fillCircle(225,225,90 ,vdgfx.White(),1);
  vdgfx.fillCircle(415,225,90 ,vdgfx.White(),1);
  vdgfx.fillRect(175,220,100,10 ,vdgfx.lightBlue(),1);
  vdgfx.fillRect(365,220,100,10 ,vdgfx.lightBlue(),1);
  vdgfx.fillRect(410,175,10,100 ,vdgfx.lightBlue(),1);
  /**
  *  for draw a text in the view could be used the function write() or print(),
  *  the text feature like color, size, position and alpha could be setted by the 
  *  functions textAlpha(), setCursor(), textColor() and textSize().
  */
  vdgfx.GFXText.setCursor(150,375);
  vdgfx.GFXText.textAlpha(1);
  vdgfx.GFXText.textSize(3);
  vdgfx.GFXText.textColor(vdgfx.lightBlue());
  vdgfx.println("ARDUINO");
  vdgfx.GFXText.setCursor(480,145);
  vdgfx.GFXText.textSize(1);
  vdgfx.println("TM");
  
  //vdgfx.GFXText.textColor(vdgfx.Blue());
  //vdgfx.println("Green");
  
  while(1){
  }
}
