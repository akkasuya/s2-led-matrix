#include "gradient.h"

void Gradient::begin() {
  speed = 128;
  spread = 32;
  accent = rgb(255, 255, 255);
}

void Gradient::frame() {
  uint16_t counter = (uint16_t)(millis() * ((speed >> 2) + 1));
  int pp = ((uint32_t)counter * NUM_LEDS) >> 16;
  int p1 = pp - NUM_LEDS;
  int p2 = pp + NUM_LEDS;
  int brd = 1 + spread / 2;
  uint16_t hueOffset = millis() / 20;

  for (int i = 0; i < NUM_LEDS; i++) {
    int val = min(abs(pp - i), min(abs(p1 - i), abs(p2 - i)));
    val = (brd > val) ? (val * 255) / brd : 255;

    uint32_t c = strip.ColorHSV((uint16_t)(i * 65536 / NUM_LEDS) + hueOffset * 256, 255, 255);
    uint16_t r = ((c >> 16) & 0xFF) * val / 255;
    uint16_t g = ((c >> 8) & 0xFF) * val / 255;
    uint16_t b = (c & 0xFF) * val / 255;
    layer0[i] = rgb(r, g, b);
  }

  uint32_t a = strip.ColorHSV(hueOffset * 256, 255, 255);
  accent = rgb((a >> 16) & 0xFF, (a >> 8) & 0xFF, a & 0xFF);
}
