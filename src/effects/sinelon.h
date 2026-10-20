#pragma once

#include "../effect.h"

class Sinelon : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  uint8_t speed;     // WLED-style speed 0-255
  uint8_t trail;     // fade amount per frame, higher = shorter trail
  uint16_t prevPos;
  Pixel accent;
  Pixel buf[NUM_LEDS];   // persistent trail buffer
};
