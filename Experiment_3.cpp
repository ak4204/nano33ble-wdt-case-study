#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Hello World / Your Name");
}

void loop() {
  // Nothing to do here
}
