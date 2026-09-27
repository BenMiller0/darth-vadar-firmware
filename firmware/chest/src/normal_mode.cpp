#include "normal_mode.hpp"
#include "blink_helpers.hpp"
#include "constants.hpp"
#include <Arduino.h>
#include <esp_system.h>

// Production chest behavior: red LEDs stay dark most of the time, then flash on
// briefly at random intervals so the panel looks alive without constant blinking.
void handleNormalModeLed(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    ledcWrite(channel, 0);
    
    const uint32_t offWindow = CHEST_RED_MAX_OFF_TIME - CHEST_RED_MIN_OFF_TIME + 1;
    int offTime = CHEST_RED_MIN_OFF_TIME + (esp_random() % offWindow);
    vTaskDelay(offTime / portTICK_PERIOD_MS);
    
    ledcWrite(channel, params->brightness);
    vTaskDelay(CHEST_RED_ON_TIME / portTICK_PERIOD_MS);
    
    ledcWrite(channel, 0);
}

void chestNormalModeLedTask(void* pvParameters) {
    LedTaskParams* params = static_cast<LedTaskParams*>(pvParameters);

    while (true) {
        handleNormalModeLed(params);
    }
}
