#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(A6, INPUT);
}

void loop() {
  int val = analogRead(A6);
  Serial.println(val);
  delay(100);
}
