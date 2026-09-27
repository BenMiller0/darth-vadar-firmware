#include <Arduino.h>
#include "constants.hpp"
#include "led_blink_task.hpp"
#include "blink_helpers.hpp"
#include "normal_mode.hpp"
#include "memory_profiler.hpp"

#if TEST_MODE
#include "test_mode.hpp"
#endif

// Belt firmware controls two red LEDs. Normal mode ignores the generic blink
// flags, but test mode still uses them to exercise digital/smooth/volatile paths.
static LedTaskParams ledParams[NUM_LEDS] = {
    {L_BELT_RED, L_BELT_RED_DELAY, VOLATILE_BLINKING, SMOOTH_BLINKING, L_BELT_RED_VOLATILITY, L_BELT_RED_BRIGHTNESS},
    {R_BELT_RED, R_BELT_RED_DELAY, VOLATILE_BLINKING, SMOOTH_BLINKING, R_BELT_RED_VOLATILITY, R_BELT_RED_BRIGHTNESS}
};

static unsigned long lastTouchTime = 0;

// Touch cycles both belt LEDs through fixed brightness levels. The first two
// low values are useful for dim indoor appearances without turning the LEDs off.
static const int brightnessLevels[] = {2, 4, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 255};
static const int numBrightnessLevels = sizeof(brightnessLevels) / sizeof(brightnessLevels[0]);
static int currentBrightnessIndex = 0;

void setup() {
#if ENABLE_SERIAL_OUTPUT
    Serial.begin(115200);
    delay(1000);
    Serial.println("Belt LED Blink Controller Starting...");
#endif
    
#if ENABLE_MEMORY_PROFILING
    initMemoryProfiler();
    
    xTaskCreate(
        memoryProfilerTask, 
        "Memory Profiler",
        4096,
        NULL, 
        1,
        NULL
    );
#endif

#if CPU_FREQUENCY_MHZ != 240
    setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
#endif
    
#if TEST_MODE
#if ENABLE_SERIAL_OUTPUT
    Serial.println("TEST MODE ENABLED - Entering test mode");
#endif
    runTestMode();
#else
#if ENABLE_SERIAL_OUTPUT
    Serial.println("NORMAL MODE - Initializing belt LED tasks");
#endif
    
    // PWM is used for both normal on/off blinking and smooth fades so brightness
    // changes work consistently.
    initializePwmPins(ledParams, NUM_LEDS);

    TaskHandle_t ledTaskHandles[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        char taskName[20];
        sprintf(taskName, "BeltLED%d", i);
        xTaskCreate(
            normalModeLedTask,
            taskName,
            1000,
            &ledParams[i],
            1,
            &ledTaskHandles[i]
        );
        
#if ENABLE_MEMORY_PROFILING
        registerTaskForProfiling(ledTaskHandles[i], taskName, 1000);
#endif
    }
#endif
}

void loop() {
#if TEST_MODE
    delay(1000);
#else
    // Capacitive touch acts like a button: one accepted touch advances one level.
    int touchValue = touchRead(TOUCH_BRIGHTNESS_PIN);
    
    if (touchValue < TOUCH_THRESHOLD) {
        unsigned long currentTime = millis();
        
        if (currentTime - lastTouchTime > TOUCH_DEBOUNCE_MS) {
            lastTouchTime = currentTime;
            
            currentBrightnessIndex = (currentBrightnessIndex + 1) % numBrightnessLevels;
            int brightness = brightnessLevels[currentBrightnessIndex];
            
            ledParams[0].brightness = brightness;
            ledParams[1].brightness = brightness;
            
            // Apply the new brightness immediately, even if a blink task is mid-cycle.
            ledcWrite(getPwmChannel(L_BELT_RED), brightness);
            ledcWrite(getPwmChannel(R_BELT_RED), brightness);
            
#if ENABLE_SERIAL_OUTPUT
            Serial.print("Touch detected! Belt red LEDs brightness set to: ");
            Serial.print(brightness);
            Serial.print(" (level ");\\ 
            Serial.print(currentBrightnessIndex + 1);
            Serial.print("/");
            Serial.print(numBrightnessLevels);
            Serial.println(")");
#endif
        }
    }
    
    delay(50);
#endif
}
