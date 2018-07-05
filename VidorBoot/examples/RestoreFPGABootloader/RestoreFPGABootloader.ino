#include "VidorBoot.h"
#include "VidorUtils.h"

void setup() {
  // Enable FPGA clock
  enableFpgaClock();

  // Initialize JTAG interface
  jtagInit();

  // Triggers a reload; this way Boot bitstream is surely running (even if another bitstream was running from RAM)
  jtagReload();
}

void loop() {
  // put your main code here, to run repeatedly:

}