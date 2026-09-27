#include "normal_mode.hpp"
#include "blink_helpers.hpp"
#include "constants.hpp"
#include <Arduino.h>

static uint32_t nextRandom(uint32_t& state) {
    state = (state * 1664525UL) + 1013904223UL;
    return state;
}

static int nextOnTimeMs(uint32_t& state) {
    const uint32_t range = RED_LED_MAX_ON_TIME - RED_LED_MIN_ON_TIME + 1;
    return RED_LED_MIN_ON_TIME + (nextRandom(state) % range);
}

static void writeLed(const LedTaskParams& params, bool on) {
    int channel = getPwmChannel(params.pin);
    ledcWrite(channel, on ? params.brightness : 0);
}

// Legacy single-LED normal handler used by the shared task in test builds.
void handleNormalModeLed(LedTaskParams* params) {
    uint32_t rngState = millis() ^ (static_cast<uint32_t>(params->pin) * 2654435761UL);

    writeLed(*params, true);
    vTaskDelay(nextOnTimeMs(rngState) / portTICK_PERIOD_MS);

    writeLed(*params, false);
    vTaskDelay(RED_LED_OFF_TIME / portTICK_PERIOD_MS);
}

// Production belt behavior: each red belt LED runs its own 10-15 second on cycle,
// then blinks off for 1 second.
void normalModeLedTask(void* pvParameters) {
    LedTaskParams* params = static_cast<LedTaskParams*>(pvParameters);
    uint32_t rngState = millis() ^ (static_cast<uint32_t>(params->pin) * 2654435761UL);

    while (true) {
        writeLed(*params, true);
        vTaskDelay(nextOnTimeMs(rngState) / portTICK_PERIOD_MS);

        writeLed(*params, false);
        vTaskDelay(RED_LED_OFF_TIME / portTICK_PERIOD_MS);
    }
}
