#include "sine.h"
#include "../wave.h"
#include "../color.h"

void Sine::begin() {
  speed = 128;
  intensity = 128;
  step = 0;
  accent = rgb(255, 255, 255);
}

void Sine::frame() {
  uint32_t colorIndex = millis() / 32;
  step += speed / 16;
  uint32_t freq = intensity / 4;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t pixBri = cubicwave8((uint8_t)(i * freq + step));
    uint8_t idx = (uint8_t)((i * colorIndex) / 255);
    layer0[i] = blendPixel(rgb(0, 0, 0), wheel(idx), pixBri);
  }
  accent = wheel((uint8_t)colorIndex);
}
