# Darth Vader Suit Firmware - Dual ESP32 Architecture

Firmware for the Star Wars Club at UC San Diego's screen-accurate Darth Vader suit, used for club promotion and filmmaking. The suit uses two independent ESP32 controllers so the belt and chest can run separately without communication wiring.

## Architecture Overview

- **Belt ESP32-S3**: Controls 2 belt red LEDs and the capacitive touch brightness sensor. Green belt LEDs are powered directly from 3.3V.
- **Chest ESP32**: Controls 3 chest red LEDs.
- **Independent operation**: Each ESP32 runs its own firmware and LED tasks.
- **Shared helper library**: Common PWM, blink-task, and optional memory-profiling code lives in `firmware/shared/`.

## Features

- 5 software-controlled LEDs split across two ESP32s
- One FreeRTOS task per software-controlled LED
- Production normal mode for Darth Vader belt and chest LED timing
- Capacitive touch brightness control for both belt red LEDs
- PWM brightness control for on/off blinking and smooth fades
- Test mode for cycling through generic digital/smooth and volatile/non-volatile blink modes
- Optional serial output and memory profiling
- Battery-minded CPU frequency configuration

## Quick Start

Edit the constants file for the controller you are building:

- **Belt ESP32-S3**: `firmware/belt/include/constants.hpp`
- **Chest ESP32**: `firmware/chest/include/constants.hpp`

The most important mode flags are:

```cpp
#define NORMAL_MODE             1    // Production costume behavior
#define TEST_MODE               0    // Set to 1 to cycle through test blink modes
#define ENABLE_SERIAL_OUTPUT    0    // Set to 1 for debug prints
```

Normal mode is intentionally simple: each LED task calls the local `handleNormalModeLed()` implementation for the belt or chest firmware. The generic `SMOOTH_BLINKING` and `VOLATILE_BLINKING` flags are still used by test mode and experimental non-normal behavior.

### Test Mode

If this flag is set to 1, the belt firmware cycles through each generic blink mode for testing:

```cpp
#define TEST_MODE               1
```

### Power Management

For battery operation, keep these settings conservative:

```cpp
#define ENABLE_SERIAL_OUTPUT           0     // Disable Serial for power savings
#define ENABLE_MEMORY_PROFILING        0     // Disable memory profiler
#define DISABLE_WIFI                   1     // Disable WiFi
#define CPU_FREQUENCY_MHZ              80    // Lower CPU frequency
#define ENABLE_LIGHT_SLEEP             1     // Enable light sleep during LED off periods
```

## Hardware Configuration

### Belt ESP32-S3 Pins

- `L_BELT_RED`: D5 / GPIO 5
- `R_BELT_RED`: D6 / GPIO 6
- `TOUCH_BRIGHTNESS_PIN`: D13 / GPIO 13
- Green belt LEDs: powered directly from 3.3V, not GPIO-controlled

### Chest ESP32 Pins

- `CHEST_RED_1`: GPIO 13
- `CHEST_RED_2`: GPIO 27
- `CHEST_RED_3`: GPIO 26

## Timing And Brightness

Normal belt timing:

```cpp
#define RED_LED_BASE_ON_TIME    10000  // Base on time for belt red LEDs
#define RED_LED_OFF_TIME        1000   // Off time for belt red LEDs
#define RED_LED_RANDOM_RANGE    3000   // Random variation range (+/- 3 seconds)
```

Normal chest timing:

```cpp
#define CHEST_RED_BASE_OFF_TIME 15000  // Base off time for chest red LEDs
#define CHEST_RED_ON_TIME       1000   // On time for chest red LEDs
#define CHEST_RED_RANDOM_RANGE  10000  // Random variation range (+/- 10 seconds)
```

Brightness uses the ESP32 LEDC PWM peripheral:

```cpp
#define PWM_FREQUENCY           5000
#define PWM_RESOLUTION          8      // 0-255 duty-cycle range

#define L_BELT_RED_BRIGHTNESS   100
#define R_BELT_RED_BRIGHTNESS   100
```

PWM is used for both smooth fades and digital-style on/off blinking so brightness limits still apply.

## Touch Brightness Control

The belt controller's capacitive touch sensor on D13 / GPIO 13 adjusts both belt red LEDs:

- Each accepted touch advances to the next brightness level.
- Debounce is controlled by `TOUCH_DEBOUNCE_MS`.
- Sensitivity is controlled by `TOUCH_THRESHOLD`.
- Brightness levels are fixed in `firmware/belt/src/main.cpp`.
- The new brightness is written immediately, even if a blink task is mid-cycle.

## Build And Upload

Requirements:

- 1x Adafruit Feather ESP32-S3 development board for the belt
- 1x ESP32 development board for the chest
- PlatformIO extension for VS Code
- USB cable for programming each ESP32

Instructions:

1. Open `firmware/belt/` in VS Code.
2. Use PlatformIO to build and upload to the belt ESP32-S3.
3. Open `firmware/chest/` in VS Code.
4. Use PlatformIO to build and upload to the chest ESP32.
5. Power both boards. No wiring is required between the two ESP32s.
6. If serial output is enabled, monitor at 115200 baud.

## Project Structure

```text
whiteout/
|-- firmware/
|   |-- belt/
|   |   |-- include/
|   |   |   |-- constants.hpp       # Belt pins, timing, and feature flags
|   |   |   |-- normal_mode.hpp     # Belt normal-mode declaration
|   |   |   `-- test_mode.hpp       # Belt test-mode declarations
|   |   |-- src/
|   |   |   |-- main.cpp            # Belt setup, tasks, and touch brightness
|   |   |   |-- normal_mode.cpp     # Belt production LED pattern
|   |   |   `-- test_mode.cpp       # Belt test-mode implementation
|   |   `-- platformio.ini
|   |-- chest/
|   |   |-- include/
|   |   |   |-- constants.hpp       # Chest pins, timing, and feature flags
|   |   |   `-- normal_mode.hpp     # Chest normal-mode declaration
|   |   |-- src/
|   |   |   |-- main.cpp            # Chest setup and tasks
|   |   |   `-- normal_mode.cpp     # Chest production LED pattern
|   |   `-- platformio.ini
|   `-- shared/
|       |-- include/
|       |   |-- blink_helpers.hpp   # PWM and blink helpers
|       |   |-- led_blink_task.hpp  # Shared FreeRTOS LED task
|       |   `-- memory_profiler.hpp # Optional memory profiling
|       |-- src/
|       |   |-- blink_helpers.cpp
|       |   |-- led_blink_task.cpp
|       |   `-- memory_profiler.cpp
|       `-- library.properties
`-- README.md
```

## Code Layout

The shared LED task contains the common task loop. In production normal mode, it delegates to each project's `handleNormalModeLed()` function:

- `firmware/belt/src/normal_mode.cpp`: belt red LEDs stay on for about 10 seconds, then blink off for 1 second.
- `firmware/chest/src/normal_mode.cpp`: chest red LEDs stay off most of the time, then blink on for 1 second.

This keeps the project-specific behavior close to the project-specific constants, while the shared helpers handle PWM setup, generic blinking, fading, and optional profiling.

## Modes

Normal mode is the production Darth Vader suit behavior:

- **Belt red LEDs**: On for about 10 seconds with slight random variation, then off for 1 second
- **Chest red LEDs**: Off for about 15 seconds with random variation, then on for 1 second
- **Green belt LEDs**: Steady on from the 3.3V rail

When `TEST_MODE` is enabled, the belt firmware cycles through the generic blink modes:

- **Digital + Non-Volatile**: Steady on/off blinking
- **Digital + Volatile**: Random on/off blinking
- **Smooth + Non-Volatile**: Steady fade in/out
- **Smooth + Volatile**: Random fade in/out

Test mode cycles through all 4 generic modes automatically, 5 seconds each. Digital blinking still uses PWM internally so brightness limits continue to apply.
