#include "dynamic_smooth.h"
#include "../color.h"

void DynamicSmooth::begin() {
  speed = 128;
  intensity = 128;
  step = 0;
  accent = rgb(255, 255, 255);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    colorIndex[i] = random(256);
    buf[i] = rgb(0, 0, 0);
  }
}

void DynamicSmooth::frame() {
  uint32_t cycleTime = 50 + (255 - speed) * 15;
  uint32_t it = millis() / cycleTime;
  if (it != step && speed != 0) {
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
      if (random(256) <= intensity) colorIndex[i] = random(256);
    }
    step = it;
  }

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    buf[i] = blendPixel(buf[i], wheel(colorIndex[i]), 16);
    layer0[i] = buf[i];
  }
  accent = buf[NUM_LEDS / 2];
}
