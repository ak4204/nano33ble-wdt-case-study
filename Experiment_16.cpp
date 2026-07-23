#include <Arduino.h>
#include "mbed.h"

using namespace mbed;
Ticker timer4;

void toggleLED() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  timer4.attach(&toggleLED, 1.0f);
}

void loop() {}
