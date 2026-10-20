#pragma once

#include "../effect.h"

class Popcorn : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  struct Kernel {
    float pos;        // < 0 = inactive
    float vel;
    uint8_t colIndex;
  };

  static const uint8_t MAX_POPCORN = 21;

  uint8_t speed;       // WLED-style gravity 0-255
  uint8_t intensity;   // WLED-style number of popcorn 0-255
  uint8_t lastColIndex;
  Kernel kernels[MAX_POPCORN];
  Pixel accent;
};
