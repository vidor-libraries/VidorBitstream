#ifndef _VIDOR_GFX_H
#define _VIDOR_GFX_H


#include "Arduino.h"
#include "VidorUtils.h"

#define VIDOR_WEIGHT 640
#define VIDOR_LENGTH 480

#define MB_DEV_GFX  0x03000000

#define GFX_PX   0
#define GFX_LN   1
#define GFX_DR   2
#define GFX_FR   3
#define GFX_DC   4
#define GFX_FC   5
#define GFX_WC   6
#define GFX_WT   7

class Vidor_GFXtext {
	public:
	Vidor_GFXtext(void);
      void textColor(uint16_t color);
      void textAlpha(uint16_t alpha=1);
      void textSize(uint16_t size);
      void setCursor(uint16_t x,uint16_t y);
      uint16_t getCursotX(void);
          uint16_t getCursotY(void);
	protected:
	    uint16_t cursor_x=0;
	    uint16_t cursor_y=12;
	    uint16_t txt_color=0;
	    uint16_t txt_alpha=1;
	    uint16_t txt_size=1;
	    friend class Vidor_GFX;
};

class Vidor_GFX:public Print {
  public:
    //Scrivere il costruttore
    Vidor_GFX(void);
    void drawPixel(uint16_t x, uint16_t y, uint16_t color, uint16_t alpha=1);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t    color, uint16_t alpha=1);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t alpha=1);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, uint16_t alpha=1);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint16_t alpha=1);
    void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint16_t alpha=1);
    void drawChar(uint16_t x, uint16_t y, uint16_t color, uint8_t size,unsigned char c, uint16_t alpha=1);
    virtual size_t write(uint8_t c);

    uint16_t Color(uint8_t r, uint8_t g, uint8_t b);
    uint16_t Red();
    uint16_t Green();
    uint16_t Blue();
    uint16_t Yellow();
    uint16_t lightBlue();
    uint16_t Purple();
    uint16_t Black();
    uint16_t Brown();
    Vidor_GFXtext GFXText;


};
#endif // _VIDOR_GFX_H
