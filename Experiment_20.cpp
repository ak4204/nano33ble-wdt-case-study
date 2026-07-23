#include <Arduino.h>
#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic ledCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, HIGH);
  if (!BLE.begin()) { while (1); }
  BLE.setLocalName("NanoLED");
  BLE.setAdvertisedService(ledService);
  ledService.addCharacteristic(ledCharacteristic);
  BLE.addService(ledService);
  ledCharacteristic.writeValue(0);
  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    while (central.connected()) {
      if (ledCharacteristic.written()) {
        char val = ledCharacteristic.value();
        if (val == 'A') digitalWrite(LEDR, LOW);
        else if (val == 'B') digitalWrite(LEDR, HIGH);
      }
    }
  }
}
