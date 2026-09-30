#include "led_blinker.hpp"
#include <esp_system.h>

static uint32_t randomMs(uint32_t minMs, uint32_t maxMs) {
    if (maxMs <= minMs) {
        return minMs;
    }

    return minMs + (esp_random() % (maxMs - minMs + 1));
}

static uint32_t nextDelayMs(const LedBlink& led, bool isOn) {
    if (isOn) {
        return randomMs(led.minOnMs, led.maxOnMs);
    }

    return randomMs(led.minOffMs, led.maxOffMs);
}

static bool timeReached(uint32_t now, uint32_t target) {
    return static_cast<int32_t>(now - target) >= 0;
}

static void writeLed(const LedBlink& led, bool isOn) {
    ledcWrite(led.channel, isOn ? led.brightness : 0);
}

static void updateLedBlinker(const LedBlinker& blinker) {
    const uint32_t now = millis();

    for (int i = 0; i < blinker.count; i++) {
        const LedBlink& led = blinker.leds[i];
        LedBlinkState& state = blinker.states[i];

        if (!timeReached(now, state.nextChangeMs)) {
            continue;
        }

        state.isOn = !state.isOn;
        state.nextChangeMs = now + nextDelayMs(led, state.isOn);
        writeLed(led, state.isOn);
    }
}

void setupLedBlinker(const LedBlinker& blinker) {
    const uint32_t now = millis();

    for (int i = 0; i < blinker.count; i++) {
        const LedBlink& led = blinker.leds[i];

        ledcSetup(led.channel, blinker.pwmFrequency, blinker.pwmResolution);
        ledcAttachPin(led.pin, led.channel);

        blinker.states[i].isOn = led.startOn;
        blinker.states[i].nextChangeMs = now + nextDelayMs(led, led.startOn);
        writeLed(led, led.startOn);
    }
}

void ledBlinkTask(void* parameters) {
    LedBlinker* blinker = static_cast<LedBlinker*>(parameters);

    while (true) {
        updateLedBlinker(*blinker);
        vTaskDelay(blinker->updateIntervalMs / portTICK_PERIOD_MS);
    }
}
