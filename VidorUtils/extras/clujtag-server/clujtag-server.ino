#include "jtag.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {}
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char buf[1024];
    int len = Serial.available();
    Serial.readBytes(buf, len);
    for (int i = 0; i < len; i++) {
      int res = jtag_execute(buf[i]);
      if (res >= 0) {
        Serial.write((uint8_t)res);
      }
    }
  }
}
