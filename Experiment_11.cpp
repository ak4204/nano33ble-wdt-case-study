#include <Arduino.h>
#include <Arduino_APDS9960.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (!APDS.begin()) { while (1); }
}

void loop() {
  if (APDS.gestureAvailable()) {
    int gesture = APDS.readGesture();
    switch (gesture) {
      case GESTURE_UP: Serial.println("UP"); break;
      case GESTURE_DOWN: Serial.println("DOWN"); break;
      case GESTURE_LEFT: Serial.println("LEFT"); break;
      case GESTURE_RIGHT: Serial.println("RIGHT"); break;
      default: break;
    }
  }
}
