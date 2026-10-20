#include "pacifica.h"
#include "../wave.h"
#include "../color.h"

// Gentle blue-green ocean waves, ported from WLED mode_pacifica (after FastLED Pacifica)
static const uint8_t ANGLE = 128;   // WLED intensity, controls wave angle
static const uint32_t FRAME_MS = 20;

static const uint32_t PAL1[16] = {
  0x002229, 0x001E2F, 0x001934, 0x001938, 0x00143F, 0x001443, 0x00B047, 0x00B04C,
  0x00004F, 0x000054, 0x000062, 0x00006F, 0x00007A, 0x000085, 0x47938A, 0x64D08E };
static const uint32_t PAL2[16] = {
  0x002229, 0x001E2F, 0x001934, 0x001938, 0x00143F, 0x001443, 0x00B047, 0x00B04C,
  0x00004F, 0x000054, 0x000062, 0x00006F, 0x00007A, 0x000085, 0x369B90, 0x4FDC9B };
static const uint32_t PAL3[16] = {
  0x00142C, 0x00193B, 0x002247, 0x002551, 0x002C5A, 0x002F63, 0x00346B, 0x003671,
  0x003B78, 0x003F7F, 0x00478E, 0x004D9C, 0x0054A9, 0x005AB4, 0x3F7FDC, 0x5A9CFF };

static Pixel unpack(uint32_t c) {
  return rgb((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

static Pixel paletteColor(const uint32_t *pal, uint8_t index, uint8_t bri) {
  uint8_t i0 = index >> 4;
  uint8_t i1 = (i0 < 15) ? i0 + 1 : 15;
  uint8_t frac = (index & 0x0F) << 4;
  Pixel c = blendPixel(unpack(pal[i0]), unpack(pal[i1]), frac);
  return rgb(scale8(c.r, bri), scale8(c.g, bri), scale8(c.b, bri));
}

static Pixel oneLayer(uint16_t i, const uint32_t *pal, uint16_t cistart,
                      uint16_t wavescale, uint8_t bri, uint16_t ioff) {
  uint16_t ci = cistart;
  uint16_t waveangle = ioff + (uint16_t)((120 + ANGLE) * i);
  uint16_t wavescale_half = (wavescale >> 1) + 20;

  uint16_t s16 = sin16(waveangle) + 32768;
  uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
  ci += cs * i;

  uint16_t sindex16 = sin16(ci) + 32768;
  uint8_t sindex8 = scale16(sindex16, 240);
  return paletteColor(pal, sindex8, bri);
}

void Pacifica::begin() {
  for (uint8_t k = 0; k < 4; k++) ci[k] = 0;
  accent = rgb(0, 150, 170);
}

void Pacifica::frame() {
  uint32_t nowMs = millis();
  uint32_t deltams = (FRAME_MS >> 2) + ((FRAME_MS * 128) >> 7);
  uint64_t deltat = (nowMs >> 2) + (((uint64_t)nowMs * 128) >> 7);
  setWaveTime((uint32_t)deltat);

  uint32_t speedfactor1 = beatsinRange(3, 179, 269);
  uint32_t speedfactor2 = beatsinRange(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  ci[0] += deltams1 * beatsin88(1011, 10, 13);
  ci[1] -= deltams21 * beatsin88(777, 8, 11);
  ci[2] -= deltams1 * beatsin88(501, 5, 7);
  ci[3] -= deltams2 * beatsin88(257, 4, 6);

  uint8_t basethreshold = beatsinRange(9, 55, 65);
  uint8_t wave = beat8(7);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    Pixel c = rgb(2, 6, 10);
    c = addPixel(c, oneLayer(i, PAL1, ci[0], beatsinRange(3, 11 * 256, 14 * 256),
                             beatsinRange(10, 70, 130), (uint16_t)(0 - beat16(301))));
    c = addPixel(c, oneLayer(i, PAL2, ci[1], beatsinRange(4, 6 * 256, 9 * 256),
                             beatsinRange(17, 40, 80), beat16(401)));
    c = addPixel(c, oneLayer(i, PAL3, ci[2], 6 * 256,
                             beatsinRange(9, 10, 38), (uint16_t)(0 - beat16(503))));
    c = addPixel(c, oneLayer(i, PAL3, ci[3], 5 * 256,
                             beatsinRange(8, 10, 28), beat16(601)));

    uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
    wave += 7;
    uint16_t l = (c.r + c.g + c.b) / 3;
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      c = addPixel(c, rgb(overage, overage2, qadd8(overage2, overage2)));
    }

    layer0[i] = c;
  }

  useRealWaveTime();
}
