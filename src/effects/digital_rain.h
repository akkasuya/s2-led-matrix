#pragma once

#include "../effect.h"

class DigitalRain : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 70; }
  Pixel accentColor() override { return rgb(0, 255, 0); }

private:
  struct Drop {
    int8_t head;   // leading (lowest) pixel row, can be off-screen
    uint8_t len;   // trail length
    uint8_t wait;  // frames to wait before respawning
  };

  Drop drops[16];

  void respawn(Drop &d);
};
