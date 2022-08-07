# zakhar_platform

Controlled from I2C and UART moving platform for Zakhar. Equipped by led indication and an MPU module (GY-91).

Part of the [Zakhar: A robotics UX project](https://github.com/an-dr/zakhar)

## Schematic

Power:

<img src="assets/schematic/platform-Power.svg" alt="platform_schematic" width="500">

Platform:

<img src="assets/schematic/platform.svg" alt="platform_schematic" width="800">

## Startup process

1. Blink by red and green leds
2. Red led
3. Motors test: forward, left, right
4. Software initialization
5. If no error detected - green led
