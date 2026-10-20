#pragma once

#include "../effect.h"

class Pride2015 : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  uint16_t sPseudotime;
  uint16_t sLastMillis;
  uint16_t sHue16;
  Pixel accent;
  Pixel state[NUM_LEDS];   // previous output, used for smoothing
};
