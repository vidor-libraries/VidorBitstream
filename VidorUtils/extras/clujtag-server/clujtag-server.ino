#include "jtag.h"

extern void enableFpgaClock();

void setup() {
  enableFpgaClock();

  Serial.begin(115200);
  while (!Serial) {}
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    int res = jtag_execute(Serial.read());

    if (res >= 0) {
      Serial.write((uint8_t)res);
    }
  }
}
