#include "sinelon.h"
#include "../wave.h"

void Sinelon::begin() {
  speed = 128;
  trail = 128;
  prevPos = 0;
  accent = rgb(255, 255, 255);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    buf[i] = rgb(0, 0, 0);
  }
}

void Sinelon::frame() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    buf[i].r = (buf[i].r * (256 - trail)) >> 8;
    buf[i].g = (buf[i].g * (256 - trail)) >> 8;
    buf[i].b = (buf[i].b * (256 - trail)) >> 8;
  }

  uint16_t pos = beatsin88((speed / 10) * 256, 0, NUM_LEDS - 1);
  uint32_t c = strip.ColorHSV((uint16_t)((uint32_t)pos * 65536 / NUM_LEDS), 255, 255);
  Pixel color = rgb((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);

  buf[pos] = color;
  if (prevPos < pos) {
    for (uint16_t i = prevPos; i < pos; i++) buf[i] = color;
  } else if (prevPos > pos) {
    for (uint16_t i = prevPos; i > pos; i--) buf[i] = color;
  }
  prevPos = pos;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = buf[i];
  }
  accent = color;
}
