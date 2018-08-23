# **NEOPIXEL** IP core

Drives multiple neopixel strings of LEDs

## Description

**NEOPIXEL** core allows driving neopixel strings via hardware. Once set up the core autonomously reads from memory and serializes data to multiple channels sharing the same setup. This basically means that a single ip core can drive more than one LEDs strings but all of them have to have the same frequency, timing and memory buffer set up. Alternatively you can add multiple instances of the core so that each one will have its own settings.

## Available Documents
* [IP Documentation](NEOPIXEL.md)
* [Soft Core Software Documentation](NEOPIXEL_sw.md)
* [Arduino Class Documentation](NEOPIXEL_ino.md)