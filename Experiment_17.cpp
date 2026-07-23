#include <Arduino.h>
#include <Arduino_APDS9960.h>
#include "mbed.h"

volatile int counter = 0;
bool limitReached = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  if (!APDS.begin()) { while (1); }
}

void loop() {
  if (APDS.proximityAvailable() && !limitReached) {
    int proximity = APDS.readProximity();
    if (proximity < 100) {
      counter++;
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      if (counter >= 15) {
        limitReached = true;
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, LOW);
      }
      delay(500);
    }
  }
}
