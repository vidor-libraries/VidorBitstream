#define PROGMEM
#include "Fonts/FreeMono12pt7b.h"
#include "Fonts/FreeMono18pt7b.h"
#include "Fonts/FreeMono24pt7b.h"
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMonoBold12pt7b.h"
#include "Fonts/FreeMonoBold18pt7b.h"
#include "Fonts/FreeMonoBold24pt7b.h"
#include "Fonts/FreeMonoBold9pt7b.h"
#include "Fonts/FreeMonoBoldOblique12pt7b.h"
#include "Fonts/FreeMonoBoldOblique18pt7b.h"
#include "Fonts/FreeMonoBoldOblique24pt7b.h"
#include "Fonts/FreeMonoBoldOblique9pt7b.h"
#include "Fonts/FreeMonoOblique12pt7b.h"
#include "Fonts/FreeMonoOblique18pt7b.h"
#include "Fonts/FreeMonoOblique24pt7b.h"
#include "Fonts/FreeMonoOblique9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeSans24pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSansBold18pt7b.h"
#include "Fonts/FreeSansBold24pt7b.h"
#include "Fonts/FreeSansBold9pt7b.h"
#include "Fonts/FreeSansBoldOblique12pt7b.h"
#include "Fonts/FreeSansBoldOblique18pt7b.h"
#include "Fonts/FreeSansBoldOblique24pt7b.h"
#include "Fonts/FreeSansBoldOblique9pt7b.h"
#include "Fonts/FreeSansOblique12pt7b.h"
#include "Fonts/FreeSansOblique18pt7b.h"
#include "Fonts/FreeSansOblique24pt7b.h"
#include "Fonts/FreeSansOblique9pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeSerif18pt7b.h"
#include "Fonts/FreeSerif24pt7b.h"
#include "Fonts/FreeSerif9pt7b.h"
#include "Fonts/FreeSerifBold12pt7b.h"
#include "Fonts/FreeSerifBold18pt7b.h"
#include "Fonts/FreeSerifBold24pt7b.h"
#include "Fonts/FreeSerifBold9pt7b.h"
#include "Fonts/FreeSerifBoldItalic12pt7b.h"
#include "Fonts/FreeSerifBoldItalic18pt7b.h"
#include "Fonts/FreeSerifBoldItalic24pt7b.h"
#include "Fonts/FreeSerifBoldItalic9pt7b.h"
#include "Fonts/FreeSerifItalic12pt7b.h"
#include "Fonts/FreeSerifItalic18pt7b.h"
#include "Fonts/FreeSerifItalic24pt7b.h"
#include "Fonts/FreeSerifItalic9pt7b.h"
#include "Fonts/Org_01.h"
#include "Fonts/Picopixel.h"
#include "Fonts/Tiny3x3a2pt7b.h"
#include "Fonts/TomThumb.h"

const GFXfont *gfxFontRepo[] = {

#ifdef FONT_9PT

#ifdef FONT_MONO
  &FreeMono9pt7b,
#ifdef FONT_ITALIC
  &FreeMonoOblique9pt7b,
#endif
#ifdef FONT_BOLD
  &FreeMonoBold9pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeMonoBoldOblique9pt7b,
#endif
#endif //FONT_MONO

#ifdef FONT_SANS
  &FreeSans9pt7b,
#ifdef FONT_ITALIC
  &FreeSansOblique9pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSansBold9pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSansBoldOblique9pt7b,
#endif
#endif //FONT_SANS

#ifdef FONT_SERIF
  &FreeSerif9pt7b,
#ifdef FONT_ITALIC
  &FreeSerifBold9pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSerifBoldItalic9pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSerifItalic9pt7b,
#endif
#endif //FONT_SERIF
#endif // FONT_9PT

#ifdef FONT_12PT
#ifdef FONT_MONO
  &FreeMono12pt7b,
#ifdef FONT_ITALIC
  &FreeMonoOblique12pt7b,
#endif
#ifdef FONT_BOLD
  &FreeMonoBold12pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeMonoBoldOblique12pt7b,
#endif
#endif //FONT_MONO

#ifdef FONT_SANS
  &FreeSans12pt7b,
#ifdef FONT_ITALIC
  &FreeSansOblique12pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSansBold12pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSansBoldOblique12pt7b,
#endif
#endif //FONT_SANS

#ifdef FONT_SERIF
  &FreeSerif12pt7b,
#ifdef FONT_ITALIC
  &FreeSerifBold12pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSerifBoldItalic12pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSerifItalic12pt7b,
#endif
#endif //FONT_SERIF
#endif // FONT_12PT

#ifdef FONT_18PT

#ifdef FONT_MONO
  &FreeMono18pt7b,
#ifdef FONT_ITALIC
  &FreeMonoOblique18pt7b,
#endif
#ifdef FONT_BOLD
  &FreeMonoBold18pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeMonoBoldOblique18pt7b,
#endif
#endif //FONT_MONO

#ifdef FONT_SANS
  &FreeSans18pt7b,
#ifdef FONT_ITALIC
  &FreeSansOblique18pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSansBold18pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSansBoldOblique18pt7b,
#endif
#endif //FONT_SANS

#ifdef FONT_SERIF
  &FreeSerif18pt7b,
#ifdef FONT_ITALIC
  &FreeSerifBold18pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSerifBoldItalic18pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSerifItalic18pt7b,
#endif
#endif //FONT_SERIF
#endif // FONT_18PT

#ifdef FONT_24PT

#ifdef FONT_MONO
  &FreeMono24pt7b,
#ifdef FONT_ITALIC
  &FreeMonoOblique24pt7b,
#endif
#ifdef FONT_BOLD
  &FreeMonoBold24pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeMonoBoldOblique24pt7b,
#endif
#endif //FONT_MONO

#ifdef FONT_SANS
  &FreeSans24pt7b,
#ifdef FONT_ITALIC
  &FreeSansOblique24pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSansBold24pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSansBoldOblique24pt7b,
#endif
#endif //FONT_SANS

#ifdef FONT_SERIF
  &FreeSerif24pt7b,
#ifdef FONT_ITALIC
  &FreeSerifBold24pt7b,
#endif
#ifdef FONT_BOLD
  &FreeSerifBoldItalic24pt7b,
#endif
#if defined(FONT_BOLD)&&defined(FONT_ITALIC)
  &FreeSerifItalic24pt7b,
#endif
#endif //FONT_SERIF
#endif // FONT_24PT

  &Org_01,
  &Picopixel,
  &Tiny3x3a2pt7b,
  &TomThumb,
};

#define GFX_NUM_FONTS (sizeof(gfxFontRepo)/sizeof(GFXfont *))
