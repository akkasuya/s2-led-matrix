#pragma once

#include "../effect.h"

class Plasma : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  uint8_t speed;       // WLED-style speed 0-255
  uint8_t intensity;   // WLED-style intensity 0-255
  uint8_t seed;        // random phase offset, 0 or 1
  Pixel accent;
};
