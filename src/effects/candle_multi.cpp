#include "candle_multi.h"
#include "../color.h"

static const uint32_t UPDATE_MS = 25;

void CandleMulti::begin() {
  speed = 96;
  intensity = 224;
  lastCall = 0;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    candles[i] = Candle{128, 130, 0};
    out[i] = rgb(0, 0, 0);
  }
}

void CandleMulti::frame() {
  uint32_t now = millis();
  if (now - lastCall >= UPDATE_MS) {
    lastCall = now;

    uint8_t speedFactor = 4;
    if (speed > 252) {
      speedFactor = 1;
    } else if (speed > 99) {
      speedFactor = 2;
    } else if (speed > 49) {
      speedFactor = 3;
    }

    uint8_t valrange = intensity;
    uint8_t rndval = valrange >> 1;
    Pixel flame = rgb(255, 147, 41);

    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      Candle &c = candles[i];

      if (c.fadeStep == 0) {
        c.s = 128;
        c.target = 130 + random(4);
        c.fadeStep = 1;
      }

      bool newTarget = false;
      if (c.target > c.s) {
        c.s = qadd8(c.s, c.fadeStep);
        if (c.s >= c.target) newTarget = true;
      } else {
        c.s = qsub8(c.s, c.fadeStep);
        if (c.s <= c.target) newTarget = true;
      }

      if (newTarget) {
        uint16_t t = random(rndval) + random(rndval);
        if (t < (rndval >> 1)) t = (rndval >> 1) + random(rndval);
        t += 255 - valrange;
        c.target = (t > 255) ? 255 : t;

        uint8_t dif = (c.target > c.s) ? c.target - c.s : c.s - c.target;
        c.fadeStep = dif >> speedFactor;
        if (c.fadeStep == 0) c.fadeStep = 1;
      }

      out[i] = blendPixel(rgb(0, 0, 0), flame, c.s);
    }
  }

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = out[i];
  }
}
