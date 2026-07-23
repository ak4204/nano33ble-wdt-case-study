#include <Arduino.h>
#include "mbed.h"
#include "rtos.h"

rtos::Thread thread1;
rtos::Thread thread2;

void led_task() {
  while (true) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    rtos::ThisThread::sleep_for(500);
  }
}

void serial_task() {
  while (true) {
    Serial.println("RTOS Task 2 Running...");
    rtos::ThisThread::sleep_for(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  thread1.start(led_task);
  thread2.start(serial_task);
}

void loop() {
  rtos::ThisThread::sleep_for(10000);
}
