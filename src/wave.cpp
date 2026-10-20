#include <math.h>
#include "wave.h"

static bool timeOverridden = false;
static uint32_t overrideTime = 0;

static uint32_t waveNow() {
  return timeOverridden ? overrideTime : millis();
}

void setWaveTime(uint32_t t) {
  timeOverridden = true;
  overrideTime = t;
}

void useRealWaveTime() {
  timeOverridden = false;
}

int16_t sin16(uint16_t theta) {
  return (int16_t)(sinf(theta * (6.2831853f / 65536.0f)) * 32767.0f);
}

uint16_t beatsin88(uint16_t bpm88, uint16_t lowest, uint16_t highest) {
  uint16_t beat = ((uint32_t)waveNow() * bpm88 * 280) >> 16;
  uint16_t beatsin = sin16(beat) + 32768;
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = ((uint32_t)beatsin * rangewidth) >> 16;
  return lowest + scaledbeat;
}

uint16_t beat16(uint16_t bpm88) {
  return (uint16_t)(((uint32_t)waveNow() * bpm88 * 280) >> 16);
}

uint8_t beat8(uint16_t bpm88) {
  return beat16(bpm88) >> 8;
}

uint8_t sin8(uint8_t theta) {
  return (sin16((uint16_t)theta << 8) + 32768) >> 8;
}

uint8_t cos8(uint8_t theta) {
  return sin8(theta + 64);
}

uint8_t triwave8(uint8_t in) {
  if (in & 0x80) in = 255 - in;
  return in << 1;
}

uint8_t quadwave8(uint8_t in) {
  float t = triwave8(in) / 255.0f;
  float e = (t < 0.5f) ? 2.0f * t * t : 1.0f - (2.0f - 2.0f * t) * (2.0f - 2.0f * t) / 2.0f;
  return (uint8_t)(e * 255.0f);
}

uint8_t cubicwave8(uint8_t in) {
  float t = triwave8(in) / 255.0f;
  float e = (t < 0.5f) ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
  return (uint8_t)(e * 255.0f);
}

int beatsinRange(uint8_t bpm, int lo, int hi) {
  uint16_t v = beatsin88((uint16_t)bpm * 256, 0, 255);
  return lo + (v * (hi - lo)) / 255;
}
