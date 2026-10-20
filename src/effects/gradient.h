#pragma once

#include "../effect.h"

class Gradient : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  uint8_t speed;     // WLED-style speed 0-255
  uint8_t spread;    // WLED-style spread 0-255
  Pixel accent;
};
