#include "solid_glitter.h"
#include "../color.h"

void SolidGlitter::begin() {
  intensity = 128;
}

void SolidGlitter::frame() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = rgb(0, 0, 0);
  }
  if (intensity > random(256)) {
    layer0[random(NUM_LEDS)] = rgb(255, 255, 255);
  }
}
