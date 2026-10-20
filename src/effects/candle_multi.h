#pragma once

#include "../effect.h"

class CandleMulti : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return rgb(255, 147, 41); }

private:
  struct Candle {
    uint8_t s;        // current brightness
    uint8_t target;   // brightness target
    uint8_t fadeStep; // movement per update, 0 = uninitialized
  };

  uint8_t speed;       // WLED-style speed 0-255
  uint8_t intensity;   // WLED-style intensity 0-255 (flicker range)
  uint32_t lastCall;
  Candle candles[NUM_LEDS];
  Pixel out[NUM_LEDS];
};
