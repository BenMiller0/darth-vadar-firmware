# Darth Vader Suit Firmware

Firmware for the Star Wars Club at UC San Diego's screen-accurate Darth Vader suit. The belt, chest, and glove each run on their own ESP32 controller, so they do not need communication wiring between them.

## Controllers

- **Belt ESP32-C3 SuperMini**: Controls 2 red belt LEDs. Green belt LEDs are powered directly from 3.3V.
- **Chest ESP32-C3 SuperMini**: Controls 3 red chest LEDs.
- **Glove ESP32-C3 SuperMini**: Reads one analog force-sensitive resistor (FSR).
- **Independent operation**: Each controller starts its firmware as soon as it runs.

There are no runtime modes, test modes, or touch brightness controls in the current firmware. The costume behavior is the firmware behavior.

## LED Behavior

The belt and chest controllers each use one FreeRTOS task to blink their software-controlled LEDs. The glove controller samples its FSR in the main loop.

**Belt red LEDs**

- Start on.
- Stay on for a random 10-15 seconds.
- Turn off for 1 second.
- Repeat independently per LED.

**Chest red LEDs**

- Start off.
- Stay off for a random 10-15 seconds.
- Turn on for 1 second.
- Repeat independently per LED.

The chest currently enables a Wi-Fi access point named `ChestKeepAlive` with
power saving disabled as a firmware-only workaround for the Miady power bank's
low-load shutdown. This increases battery use. Disable it with
`CHEST_BATTERY_KEEPALIVE 0` when using a proper keep-alive load or a battery
pack that does not shut down at low current.

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

**Glove ESP32-C3 SuperMini**

- FSR analog output: GPIO 0 / ADC1 channel 0
- Samples every 100 ms and prints `raw, millivolts` over native USB CDC and UART0 at 115200 baud
- Use a voltage divider: 3.3V -> FSR -> GPIO 0 -> fixed resistor -> GND
- A 10 kOhm fixed resistor is a reasonable starting point
- Keep the GPIO 0 voltage between 0V and 3.3V

All three firmware projects use PlatformIO's `esp32-c3-devkitm-1` target, which is
compatible with the common ESP32-C3 SuperMini boards. Connect each LED through
its appropriate current-limiting resistor, and connect LED ground to board
ground. If the physical wiring uses different GPIOs, update
`firmware/chest/include/constants.hpp`.

## Configuration

Edit the constants file for the controller you are building:

- Belt: `firmware/belt/include/constants.hpp`
- Chest: `firmware/chest/include/constants.hpp`
- Glove: `firmware/glove/include/constants.hpp`

Useful values:

```cpp
#define PWM_FREQUENCY           5000
#define PWM_RESOLUTION          8
#define ENABLE_SERIAL_OUTPUT    0
#define CPU_FREQUENCY_MHZ       160
```

Belt timing:

```cpp
#define RED_LED_MIN_ON_TIME     10000
#define RED_LED_MAX_ON_TIME     15000
#define RED_LED_OFF_TIME        1000
```

Chest timing:

```cpp
#define CHEST_RED_MIN_OFF_TIME  10000
#define CHEST_RED_MAX_OFF_TIME  15000
#define CHEST_RED_ON_TIME       1000
```

## Build

Requirements:

- PlatformIO
- 1x ESP32-C3 SuperMini development board for the belt
- 1x ESP32-C3 SuperMini development board for the chest
- 1x ESP32-C3 SuperMini development board for the glove
- USB cable for programming each board

Build from the repo root:

```powershell
pio run -d firmware/belt
pio run -d firmware/chest
pio run -d firmware/glove
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
|   |-- glove/
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
