#include "blends.h"
#include "../wave.h"
#include "../color.h"

void Blends::begin() {
  speed = 128;
  intensity = 128;
  accent = rgb(255, 255, 255);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    buf[i] = rgb(0, 0, 0);
  }
}

void Blends::frame() {
  uint8_t blendSpeed = map(intensity, 0, 255, 10, 128);
  uint32_t shift = (millis() * ((speed >> 3) + 1)) >> 8;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t idx = (uint8_t)(shift + quadwave8((uint8_t)((i + 1) * 16)));
    buf[i] = blendPixel(buf[i], wheel(idx), blendSpeed);
    shift += 3;
  }

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = buf[i];
  }
  accent = wheel((uint8_t)shift);
}
