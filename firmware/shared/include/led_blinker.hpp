#ifndef LED_BLINKER_HPP
#define LED_BLINKER_HPP

#include <Arduino.h>

struct LedBlink {
    int pin;
    int channel;
    int brightness;
    bool startOn;
    uint32_t minOnMs;
    uint32_t maxOnMs;
    uint32_t minOffMs;
    uint32_t maxOffMs;
};

struct LedBlinkState {
    bool isOn;
    uint32_t nextChangeMs;
};

struct LedBlinker {
    const LedBlink* leds;
    LedBlinkState* states;
    int count;
    int pwmFrequency;
    int pwmResolution;
    uint32_t updateIntervalMs;
};

void setupLedBlinker(const LedBlinker& blinker);
void ledBlinkTask(void* parameters);

#endif // LED_BLINKER_HPP
