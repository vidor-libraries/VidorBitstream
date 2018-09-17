#include <VidorBoot.h>
#include <VidorUtils.h>
#include <VidorJTAG.h>

void setup() {
  // Enable FPGA clock
  enableFpgaClock();

  // Initialize JTAG interface
  VidorJTAG.begin();

  // Triggers a reload; this way Boot bitstream is surely running (even if another bitstream was running from RAM)
  VidorJTAG.reset();
}

void loop() {
}
