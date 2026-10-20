#pragma once

#include "../effect.h"

class RippleRainbow : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  struct Ripple {
    uint8_t state;   // 0 = inactive
    uint8_t color;   // rainbow index
    uint16_t pos;    // origin LED index
  };

  static const uint8_t MAX_RIPPLES = NUM_LEDS / 4 + 1;

  uint8_t speed;       // WLED-style speed 0-255 (propagation & decay)
  uint8_t intensity;   // WLED-style intensity 0-255 (spawn rate)
  uint8_t aux0;        // background rainbow position, drifts toward aux1
  uint8_t aux1;        // background rainbow target
  Ripple ripples[MAX_RIPPLES];
  Pixel accent;

  void drawRipples();
};
