#pragma once

#include "../effect.h"

class SolidGlitter : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return rgb(255, 255, 255); }

private:
  uint8_t intensity;   // WLED-style glitter density 0-255
};
