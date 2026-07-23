#include <Arduino.h>

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
}

void loop() {
  digitalWrite(LEDR, LOW);
  delay(1000);
  digitalWrite(LEDR, HIGH);
  
  digitalWrite(LEDG, LOW);
  delay(1000);
  digitalWrite(LEDG, HIGH);
  
  digitalWrite(LEDB, LOW);
  delay(1000);
  digitalWrite(LEDB, HIGH);
}
