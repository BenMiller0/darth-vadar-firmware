#include "led_blink_task.hpp"
#include "blink_helpers.hpp"
#include "normal_mode.hpp"
#include <Arduino.h>

// One FreeRTOS task runs per LED. In normal operation, each project supplies its
// own handleNormalModeLed() implementation so the shared task does not need to
// know whether it was compiled for the belt or chest controller.
void ledBlinkTask(void* pvParameters) {
    LedTaskParams* params = static_cast<LedTaskParams*>(pvParameters);
    
    // Offset the random sequence for each LED so matching pins do not blink in lockstep.
    randomSeed(millis() + params->pin);
    int channel = getPwmChannel(params->pin);

#if NORMAL_MODE && !TEST_MODE
    // Normal mode is the production costume behavior. It bypasses the generic
    // blink modes because belt and chest LEDs intentionally have different timing.
    while (true) {
        handleNormalModeLed(params);
    }
#endif

    // The generic modes below are mainly for TEST_MODE and future experiments.
    if (params->volatilityMultiplier == 0.0f) {
        handleSolidLED(params, channel);
        return;
    }

    while (true) {
        if (params->smoothBlinking == 1) {
            handleSmoothBlinking(params, channel);
        } else {
            handleDigitalBlinking(params);
        }
    }
}
