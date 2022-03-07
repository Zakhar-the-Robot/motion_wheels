---
title: Motors
---

[![view - Documentation](https://img.shields.io/badge/Source%20Code-GitHub-blue)](https://github.com/Zakhar-the-Robot/motors "Go to repository")

Moving platform for Zakhar. Equipped by led indication and an MPU module (GY-91).

## Schematic

Power:

<img src="_index_power.svg" alt="platform_schematic" width="500">

Platform:

<img src="_index_platform.svg" alt="platform_schematic" width="800">

## Startup Sequence

1. Blink by red and green leds
2. Red led
3. Motors test: forward, left, right
4. Software initialization
5. If no error detected - green led
