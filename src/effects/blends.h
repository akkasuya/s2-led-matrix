#pragma once

#include "../effect.h"

class Blends : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  uint8_t speed;       // WLED-style shift speed 0-255
  uint8_t intensity;   // WLED-style blend speed 0-255
  Pixel buf[NUM_LEDS];
  Pixel accent;
};
