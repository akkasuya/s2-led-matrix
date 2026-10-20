#pragma once

#include "../effect.h"

class Drip : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return rgb(0, 120, 255); }

private:
  struct Drop {
    float pos;
    float vel;
    int col;
    uint8_t colIndex;   // 0 init, 1 forming, 2 falling, 5 bouncing
  };

  uint8_t speed;       // WLED-style gravity 0-255
  uint8_t intensity;   // WLED-style number of drips 0-255
  Drop drops[4];
};
