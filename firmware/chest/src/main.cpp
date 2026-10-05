#include <Arduino.h>
#include "constants.hpp"
#include "led_blinker.hpp"

#if CHEST_BATTERY_KEEPALIVE
#include <WiFi.h>
#endif

static const LedBlink leds[NUM_LEDS] = {
#if CHEST_INVERT_ON_OFF
    {CHEST_RED_1, 0, CHEST_RED_1_BRIGHTNESS, true, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME},
    {CHEST_RED_2, 1, CHEST_RED_2_BRIGHTNESS, true, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME},
    {CHEST_RED_3, 2, CHEST_RED_3_BRIGHTNESS, true, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME}
#else
    {CHEST_RED_1, 0, CHEST_RED_1_BRIGHTNESS, false, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME},
    {CHEST_RED_2, 1, CHEST_RED_2_BRIGHTNESS, false, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME},
    {CHEST_RED_3, 2, CHEST_RED_3_BRIGHTNESS, false, CHEST_RED_ON_TIME, CHEST_RED_ON_TIME, CHEST_RED_MIN_OFF_TIME, CHEST_RED_MAX_OFF_TIME}
#endif
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

#if CHEST_BATTERY_KEEPALIVE
static void enableBatteryKeepAlive() {
    WiFi.mode(WIFI_AP);
    WiFi.setSleep(false);
    WiFi.softAP("ChestKeepAlive", nullptr, 1, false, 1);
}
#endif

void setup() {
#if ENABLE_SERIAL_OUTPUT
    Serial.begin(115200);
    delay(1000);
    Serial.println("Chest LED controller starting...");
#endif

#if CPU_FREQUENCY_MHZ != 240
    setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
#endif

#if CHEST_BATTERY_KEEPALIVE
    enableBatteryKeepAlive();
#endif

    setupLedBlinker(blinker);
    xTaskCreate(ledBlinkTask, "Chest LEDs", 2048, &blinker, 1, nullptr);
}

void loop() {
    delay(1000);
}
