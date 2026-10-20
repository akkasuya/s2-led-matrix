#include "color.h"

Pixel wheel(uint8_t pos) {
  uint32_t c = strip.ColorHSV((uint16_t)pos * 256, 255, 255);
  return rgb((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

Pixel blendPixel(Pixel a, Pixel b, uint8_t amount) {
  uint16_t ia = 255 - amount;
  return rgb((a.r * ia + b.r * amount) / 255,
             (a.g * ia + b.g * amount) / 255,
             (a.b * ia + b.b * amount) / 255);
}

Pixel addPixel(Pixel a, Pixel b) {
  return rgb(qadd8(a.r, b.r), qadd8(a.g, b.g), qadd8(a.b, b.b));
}

uint8_t scale8(uint8_t a, uint8_t b) {
  return ((uint16_t)a * b) >> 8;
}

uint16_t scale16(uint16_t a, uint16_t b) {
  return ((uint32_t)a * b) >> 16;
}

uint8_t qadd8(uint8_t a, uint8_t b) {
  uint16_t s = (uint16_t)a + b;
  return (s > 255) ? 255 : s;
}

uint8_t qsub8(uint8_t a, uint8_t b) {
  return (a > b) ? a - b : 0;
}
