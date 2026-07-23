#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  
  if (!IMU.begin()) {
    while (1);
  }
}

void loop() {
  float x, y, z;
  
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    
    if (x > 0.5) digitalWrite(LEDR, LOW);
    else if (x < -0.5) digitalWrite(LEDG, LOW);
    else if (y > 0.5) digitalWrite(LEDB, LOW);
    else if (y < -0.5) { digitalWrite(LEDR, LOW); digitalWrite(LEDG, LOW); }
  }
  delay(100);
}
