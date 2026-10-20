#include "plasma.h"
#include "../wave.h"

static uint8_t qsub8(uint8_t a, uint8_t b) {
  return (a > b) ? a - b : 0;
}

void Plasma::begin() {
  speed = 128;
  intensity = 128;
  seed = random(0, 2);
  accent = rgb(255, 255, 255);
}

void Plasma::frame() {
  int thisPhase = beatsinRange(6 + seed, -64, 64);
  int thatPhase = beatsinRange(7 + seed, -64, 64);
  int braking = beatsinRange(7, 0, 128 - (intensity >> 1));

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t colorIndex = cubicwave8((i * (2 + 3 * (speed >> 5)) + thisPhase) & 0xFF) / 2
                       + cos8((i * (1 + 2 * (speed >> 5)) + thatPhase) & 0xFF) / 2;
    uint8_t bri = qsub8(colorIndex, braking);

    uint32_t c = strip.ColorHSV((uint16_t)colorIndex * 256, 255, bri);
    layer0[i] = rgb((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
  }

  accent = layer0[NUM_LEDS / 2];
}
