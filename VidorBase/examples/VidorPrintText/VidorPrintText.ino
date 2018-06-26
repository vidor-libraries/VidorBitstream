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

/**
 * put your main code here, to run repeatedly:
 */

void loop()
{
  vdgfx.fillRect(0,0,640,480,65535,1);
  vdgfx.GFXText.setCursor(320,240);
  vdgfx.drawCircle(320,240,200 ,31744,1);//disegno oltre i margini del monitor ricomincia da 0
  vdgfx.GFXText.textAlpha(1);
  vdgfx.GFXText.textSize(3);
  vdgfx.GFXText.textColor(vdgfx.Green());
  vdgfx.println("Green");
  vdgfx.GFXText.setCursor(320,290);
  vdgfx.GFXText.textColor(vdgfx.Blue());
  vdgfx.println("Blue");
  while(1){
  }
}
