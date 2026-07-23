#include <Arduino.h>
#include <ArduinoBLE.h>

BLEService sensorService("181A");
BLEUnsignedIntCharacteristic sensorCharacteristic("2A6E", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  if (!BLE.begin()) { while (1); }
  BLE.setLocalName("Nano33BLE");
  BLE.setAdvertisedService(sensorService);
  sensorService.addCharacteristic(sensorCharacteristic);
  BLE.addService(sensorService);
  sensorCharacteristic.writeValue(0);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    while (central.connected()) {
      int simulatedData = analogRead(A0);
      sensorCharacteristic.writeValue(simulatedData);
      delay(1000);
    }
  }
}
