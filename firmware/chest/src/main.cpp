#include <Arduino.h>
#include "constants.hpp"
#include "led_blinker.hpp"

static const LedBlink leds[NUM_LEDS] = {
    {CHEST_RED_1, 0, CHEST_RED_1_BRIGHTNESS, false, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME},
    {CHEST_RED_2, 1, CHEST_RED_2_BRIGHTNESS, false, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME},
    {CHEST_RED_3, 2, CHEST_RED_3_BRIGHTNESS, false, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME}
};

static LedBlinkState ledStates[NUM_LEDS] = {};

static LedBlinker blinker = {
    leds,
    ledStates,
    NUM_LEDS,
    PWM_FREQUENCY,
    PWM_RESOLUTION,
    LED_UPDATE_INTERVAL_MS
};

void setup() {
#if ENABLE_SERIAL_OUTPUT
    Serial.begin(115200);
    delay(1000);
    Serial.println("Chest LED controller starting...");
#endif

#if CPU_FREQUENCY_MHZ != 240
    setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
#endif

    setupLedBlinker(blinker);
    xTaskCreate(ledBlinkTask, "Chest LEDs", 2048, &blinker, 1, nullptr);
}

void loop() {
    delay(1000);
}
