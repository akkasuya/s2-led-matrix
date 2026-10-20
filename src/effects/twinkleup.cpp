#include "twinkleup.h"
#include "../wave.h"
#include "../color.h"

static uint8_t nextRand8(uint16_t &seed) {
  seed = (uint16_t)(seed * 2053) + 13849;
  return seed >> 8;
}

void TwinkleUp::begin() {
  speed = 128;
  intensity = 128;
  accent = rgb(255, 255, 255);
}

void TwinkleUp::frame() {
  uint32_t now = millis();
  uint16_t seed = 535;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t ranstart = nextRand8(seed);
    uint8_t pixBri = sin8((uint8_t)(ranstart + 16 * now / (256 - speed)));
    if (nextRand8(seed) > intensity) pixBri = 0;
    uint8_t idx = (uint8_t)(nextRand8(seed) + now / 100);
    layer0[i] = blendPixel(rgb(0, 0, 0), wheel(idx), pixBri);
  }

  accent = wheel((uint8_t)(now / 100));
}
