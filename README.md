# Lightbox
## Purpose
Lightbox controls analog LED strips in the easyGrade card capture box.

## Associated projects

## Target configuration
- Set parameter configuration using config.h

## Bill of Materials (BOM)
  - 1 or more: Adafruit Rotary Encoder 377b
    - includes rotary encoder and button
  - LED strip; any supported by FastLED or Adafruit NeoPixel (for RGBW)
  - optional: Blinkytape (Arduino Leonardo with 60 WS2811)
  - 1X: MCU if not Blinkytape
  - 1 protoboard
  - wire

## Pinouts
  - buttons to digital pin, ground
  - rotary encoder to interupt enabled pins, ground
  - LED strip to data pin(s), independent 5v, power GND + Arduino GND

## Information Sources
- See Evernote on wiring for LED strips.

## Software Dependencies