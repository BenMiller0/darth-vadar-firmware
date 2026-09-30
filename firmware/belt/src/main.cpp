#include <Arduino.h>
#include "constants.hpp"
#include "led_blinker.hpp"

static const LedBlink leds[NUM_LEDS] = {
    {L_BELT_RED, 0, L_BELT_RED_BRIGHTNESS, true, RED_LED_MIN_ON_TIME, RED_LED_MAX_ON_TIME, RED_LED_OFF_TIME, RED_LED_OFF_TIME},
    {R_BELT_RED, 1, R_BELT_RED_BRIGHTNESS, true, RED_LED_MIN_ON_TIME, RED_LED_MAX_ON_TIME, RED_LED_OFF_TIME, RED_LED_OFF_TIME}
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
    Serial.println("Belt LED controller starting...");
#endif

#if CPU_FREQUENCY_MHZ != 240
    setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
#endif

    setupLedBlinker(blinker);
    xTaskCreate(ledBlinkTask, "Belt LEDs", 2048, &blinker, 1, nullptr);
}

void loop() {
    delay(1000);
}
