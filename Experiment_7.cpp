#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  float x, y, z;
  
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    if (x > 0.5) Serial.println("Left");
    else if (x < -0.5) Serial.println("Right");
    else if (y > 0.5) Serial.println("Front");
    else if (y < -0.5) Serial.println("Back");
  }
  delay(200);
}
