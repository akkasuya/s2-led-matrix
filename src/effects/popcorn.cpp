#include <math.h>
#include "popcorn.h"
#include "../color.h"

void Popcorn::begin() {
  speed = 128;
  intensity = 128;
  lastColIndex = 0;
  accent = rgb(255, 255, 255);
  for (uint8_t k = 0; k < MAX_POPCORN; k++) {
    kernels[k] = Kernel{-1.0f, 0.0f, 0};
  }
}

void Popcorn::frame() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = rgb(0, 0, 0);
  }

  float gravity = (-0.0001f - speed / 200000.0f) * NUM_LEDS;
  uint16_t numPopcorn = intensity * MAX_POPCORN / 255;
  if (numPopcorn == 0) numPopcorn = 1;

  for (uint16_t i = 0; i < numPopcorn; i++) {
    Kernel &k = kernels[i];

    if (k.pos >= 0.0f) {
      k.pos += k.vel;
      k.vel += gravity;
    } else if (random(256) < 2) {
      k.pos = 0.01f;
      unsigned peakHeight = 128 + random(128);
      peakHeight = (peakHeight * (NUM_LEDS - 1)) >> 8;
      k.vel = sqrtf(-2.0f * gravity * peakHeight);
      k.colIndex = random(256);
      lastColIndex = k.colIndex;
    }

    if (k.pos >= 0.0f) {
      unsigned led = (unsigned)k.pos;
      if (led < NUM_LEDS) layer0[led] = wheel(k.colIndex);
    }
  }

  accent = wheel(lastColIndex);
}
