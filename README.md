# Darth Vader Suit Firmware

Firmware for the Star Wars Club at UC San Diego's screen-accurate Darth Vader suit. The belt and chest each run on their own ESP32 controller, so they do not need communication wiring between them.

## Controllers

- **Belt ESP32-C3 SuperMini**: Controls 2 red belt LEDs. Green belt LEDs are powered directly from 3.3V.
- **Chest ESP32-C3 SuperMini**: Controls 3 red chest LEDs.
- **Independent operation**: Each controller starts its LED pattern as soon as the firmware runs.

There are no runtime modes, test modes, or touch brightness controls in the current firmware. The costume behavior is the firmware behavior.

## LED Behavior

Each controller uses one FreeRTOS task to blink all of its software-controlled LEDs.

**Belt red LEDs**

- Start on.
- Stay on for a random 10-15 seconds.
- Turn off for 1 second.
- Repeat independently per LED.

**Chest red LEDs**

- Start off.
- Stay off for a random 5-20 seconds.
- Turn on for 1 second.
- Repeat independently per LED.

PWM is used for LED output so the fixed brightness values in each controller's `constants.hpp` can be set with LEDC duty cycle.

## Hardware Pins

**Belt ESP32-C3 SuperMini**

- `L_BELT_RED`: GPIO 3
- `R_BELT_RED`: GPIO 4
- Green belt LEDs: powered directly from 3.3V, not GPIO-controlled

**Chest ESP32-C3 SuperMini**

- `CHEST_RED_1`: GPIO 5
- `CHEST_RED_2`: GPIO 6
- `CHEST_RED_3`: GPIO 7

Both firmware projects use PlatformIO's `esp32-c3-devkitm-1` target, which is
compatible with the common ESP32-C3 SuperMini boards. Connect each LED through
its appropriate current-limiting resistor, and connect LED ground to board
ground. If the physical wiring uses different GPIOs, update
`firmware/chest/include/constants.hpp`.

## Configuration

Edit the constants file for the controller you are building:

- Belt: `firmware/belt/include/constants.hpp`
- Chest: `firmware/chest/include/constants.hpp`

Useful values:

```cpp
#define PWM_FREQUENCY           5000
#define PWM_RESOLUTION          8
#define ENABLE_SERIAL_OUTPUT    0
#define CPU_FREQUENCY_MHZ       80
```

Belt timing:

```cpp
#define RED_LED_MIN_ON_TIME     10000
#define RED_LED_MAX_ON_TIME     15000
#define RED_LED_OFF_TIME        1000
```

Chest timing:

```cpp
#define CHEST_RED_MIN_OFF_TIME  5000
#define CHEST_RED_MAX_OFF_TIME  20000
#define CHEST_RED_ON_TIME       1000
```

## Build

Requirements:

- PlatformIO
- 1x ESP32-C3 SuperMini development board for the belt
- 1x ESP32-C3 SuperMini development board for the chest
- USB cable for programming each board

Build from the repo root:

```powershell
pio run -d firmware/belt
pio run -d firmware/chest
```

Upload with PlatformIO from each firmware directory or by using the PlatformIO VS Code extension.

## Project Structure

```text
whiteout/
|-- firmware/
|   |-- belt/
|   |   |-- include/
|   |   |   `-- constants.hpp
|   |   |-- src/
|   |   |   `-- main.cpp
|   |   `-- platformio.ini
|   |-- chest/
|   |   |-- include/
|   |   |   `-- constants.hpp
|   |   |-- src/
|   |   |   `-- main.cpp
|   |   `-- platformio.ini
|   `-- shared/
|       |-- include/
|       |   `-- led_blinker.hpp
|       |-- src/
|       |   `-- led_blinker.cpp
|       `-- library.properties
`-- README.md
```

`firmware/shared/` contains the timed LED blink state machine used by both controllers. The belt and chest firmware only provide their LED pin/timing configs, then start the shared `ledBlinkTask()`.
