#include <Arduino.h>
#include <PDM.h>

short sampleBuffer[256];
volatile int samplesRead;

void onPDMdata() {
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  PDM.onReceive(onPDMdata);
  if (!PDM.begin(1, 16000)) { while (1); }
}

void loop() {
  if (samplesRead) {
    long sum = 0;
    for (int i = 0; i < samplesRead; i++) {
      sum += abs(sampleBuffer[i]);
    }
    int average = sum / samplesRead;
    Serial.println(average);
    samplesRead = 0;
  }
}
