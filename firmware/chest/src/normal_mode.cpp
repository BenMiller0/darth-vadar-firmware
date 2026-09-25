#include "normal_mode.hpp"
#include "blink_helpers.hpp"
#include "constants.hpp"
#include <Arduino.h>

// Production chest behavior: red LEDs stay dark most of the time, then flash on
// briefly at random intervals so the panel looks alive without constant blinking.
void handleNormalModeLed(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    ledcWrite(channel, 0);
    
    // Clamp the randomized delay so a negative swing never makes the blink frantic.
    int offTime = CHEST_RED_BASE_OFF_TIME + random(-CHEST_RED_RANDOM_RANGE, CHEST_RED_RANDOM_RANGE);
    offTime = max(5000, offTime);
    vTaskDelay(offTime / portTICK_PERIOD_MS);
    
    ledcWrite(channel, params->brightness);
    vTaskDelay(CHEST_RED_ON_TIME / portTICK_PERIOD_MS);
    
    ledcWrite(channel, 0);
}
