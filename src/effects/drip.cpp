#include "drip.h"
#include "../color.h"

static const Pixel WATER = {0, 120, 255, 255};

static void setLed(int index, Pixel color) {
  if (index < 0 || index >= NUM_LEDS) return;
  layer0[index] = color;
}

static uint8_t clamp8(int v) {
  return (v > 255) ? 255 : (v < 0 ? 0 : v);
}

void Drip::begin() {
  speed = 128;
  intensity = 128;
  for (uint8_t j = 0; j < 4; j++) {
    drops[j] = Drop{0, 0, 0, 0};
  }
}

void Drip::frame() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = rgb(0, 0, 0);
  }

  uint8_t numDrops = 1 + (intensity >> 6);
  float gravity = (-0.0005f - speed / 50000.0f) * max(1, NUM_LEDS - 1);
  int sourcedrop = 12;
  Pixel black = rgb(0, 0, 0);

  for (uint8_t j = 0; j < numDrops; j++) {
    Drop &d = drops[j];

    if (d.colIndex == 0) {
      d.pos = NUM_LEDS - 1;
      d.vel = 0;
      d.col = sourcedrop;
      d.colIndex = 1;
    }

    setLed(NUM_LEDS - 1, blendPixel(black, WATER, clamp8(sourcedrop)));

    if (d.colIndex == 1) {
      d.col = clamp8(d.col);
      setLed((int)d.pos, blendPixel(black, WATER, clamp8(d.col)));
      d.col += map(speed, 0, 255, 1, 6);
      if (random(256) < d.col / 10) {
        d.colIndex = 2;
        d.col = 255;
      }
    }

    if (d.colIndex > 1) {
      if (d.pos > 0) {
        d.pos += d.vel;
        if (d.pos < 0) d.pos = 0;
        d.vel += gravity;

        for (int i = 1; i < 7 - d.colIndex; i++) {
          int p = constrain((int)d.pos + i, 0, NUM_LEDS - 1);
          setLed(p, blendPixel(black, WATER, clamp8(d.col / i)));
        }

        if (d.colIndex > 2) {
          setLed(0, blendPixel(WATER, black, clamp8(d.col)));
        }
      } else {
        if (d.colIndex > 2) {
          d.colIndex = 0;
          d.col = sourcedrop;
        } else {
          if (d.colIndex == 2) {
            d.vel = -d.vel / 4;
            d.pos += d.vel;
          }
          d.col = sourcedrop * 2;
          d.colIndex = 5;
        }
      }
    }
  }
}
