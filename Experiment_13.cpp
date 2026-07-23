#include <Arduino.h>
#include <Arduino_APDS9960.h>

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  if (!APDS.begin()) { while (1); }
}

void loop() {
  if (APDS.proximityAvailable()) {
    int proximity = APDS.readProximity();
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    if (proximity > 200) digitalWrite(LEDB, LOW);
    else if (proximity >= 100) digitalWrite(LEDG, LOW);
    else digitalWrite(LEDR, LOW);
  }
  delay(100);
}
