#include "chunchun.h"
#include "../wave.h"
#include "../color.h"

void Chunchun::begin() {
  speed = 128;
  intensity = 128;
  accent = rgb(255, 255, 255);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    buf[i] = rgb(0, 0, 0);
  }
}

void Chunchun::frame() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    buf[i].r = (buf[i].r * 254) >> 8;
    buf[i].g = (buf[i].g * 254) >> 8;
    buf[i].b = (buf[i].b * 254) >> 8;
  }

  uint32_t counter = millis() * (6 + (speed >> 4));
  uint32_t numBirds = 2 + (NUM_LEDS >> 3);
  uint32_t span = ((uint32_t)intensity << 8) / numBirds;

  for (uint32_t i = 0; i < numBirds; i++) {
    counter -= span;
    uint32_t megumin = (uint32_t)(sin16((uint16_t)counter) + 0x8000);
    uint32_t bird = (megumin * NUM_LEDS) >> 16;
    if (bird >= NUM_LEDS) bird = NUM_LEDS - 1;
    buf[bird] = wheel((uint8_t)((i * 255) / numBirds));
  }

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = buf[i];
  }
  accent = wheel((uint8_t)(counter >> 8));
}
