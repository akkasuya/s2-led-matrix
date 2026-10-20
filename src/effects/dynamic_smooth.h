#pragma once

#include "../effect.h"

class DynamicSmooth : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  uint8_t speed;       // WLED-style speed 0-255 (0 = no color change)
  uint8_t intensity;   // WLED-style chance of each LED changing color
  uint32_t step;       // last color-change interval
  uint8_t colorIndex[NUM_LEDS];
  Pixel buf[NUM_LEDS]; // persistent output for smooth blending
  Pixel accent;
};
