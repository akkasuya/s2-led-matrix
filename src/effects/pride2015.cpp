#include "pride2015.h"
#include "../wave.h"

static uint8_t blendChannel(uint8_t cur, uint8_t target) {
  int16_t diff = (int16_t)target - (int16_t)cur;
  return cur + ((diff * 64) >> 8);
}

void Pride2015::begin() {
  sPseudotime = 0;
  sLastMillis = millis();
  sHue16 = 0;
  accent = rgb(255, 255, 255);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    state[i] = rgb(0, 0, 0);
  }
}

void Pride2015::frame() {
  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, 25 * 256, 40 * 256);
  uint8_t msmultiplier = beatsin88(147, 23, 60);
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t hue16 = sHue16;
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;
    uint32_t bri16 = (uint32_t)b16 * b16;
    bri16 >>= 16;
    uint8_t bri8 = ((uint32_t)bri16 * brightdepth) >> 16;
    bri8 += (255 - brightdepth);

    uint32_t c = strip.ColorHSV((uint16_t)hue8 * 256, sat8, bri8);
    Pixel target = rgb((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);

    Pixel &s = state[i];
    s = rgb(blendChannel(s.r, target.r),
            blendChannel(s.g, target.g),
            blendChannel(s.b, target.b));
    layer0[i] = s;
  }

  uint32_t a = strip.ColorHSV(sHue16, 255, 255);
  accent = rgb((a >> 16) & 0xFF, (a >> 8) & 0xFF, a & 0xFF);
}
