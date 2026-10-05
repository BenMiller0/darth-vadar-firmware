#include <Arduino.h>
#include "constants.hpp"

static void printStartupMessage() {
    Serial.println("Glove FSR reader starting...");
    Serial0.println("Glove FSR reader starting...");
    Serial.println("raw, millivolts");
    Serial0.println("raw, millivolts");
}

void setup() {
    Serial.begin(115200);
    Serial0.begin(115200);
    // Give the USB CDC monitor time to enumerate, without blocking startup.
    delay(1000);

    analogReadResolution(ADC_RESOLUTION_BITS);
    analogSetPinAttenuation(FSR_PIN, ADC_ATTENUATION);
    pinMode(FSR_PIN, INPUT);

    printStartupMessage();
}

void loop() {
    const uint16_t rawValue = analogRead(FSR_PIN);
    const uint32_t millivolts = analogReadMilliVolts(FSR_PIN);

    Serial.printf("%u, %lu\n", rawValue, static_cast<unsigned long>(millivolts));
    Serial0.printf("%u, %lu\n", rawValue, static_cast<unsigned long>(millivolts));
    delay(FSR_SAMPLE_INTERVAL_MS);
}
