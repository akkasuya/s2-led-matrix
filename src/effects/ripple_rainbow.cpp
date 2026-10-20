#include "ripple_rainbow.h"
#include "../wave.h"

static Pixel wheel(uint8_t pos) {
  uint32_t c = strip.ColorHSV((uint16_t)pos * 256, 255, 255);
  return rgb((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

static uint8_t scale8(uint8_t a, uint8_t b) {
  return ((uint16_t)a * b) >> 8;
}

static uint8_t mix8(uint8_t a, uint8_t b, uint8_t amount) {
  return (a * (255 - amount) + b * amount) / 255;
}

static Pixel mixPixel(Pixel a, Pixel b, uint8_t amount) {
  return rgb(mix8(a.r, b.r, amount), mix8(a.g, b.g, amount), mix8(a.b, b.b, amount));
}

static Pixel getLed(int index) {
  if (index < 0 || index >= NUM_LEDS) return rgb(0, 0, 0);
  return layer0[index];
}

static void setLed(int index, Pixel color) {
  if (index < 0 || index >= NUM_LEDS) return;
  layer0[index] = color;
}

void RippleRainbow::begin() {
  speed = 128;
  intensity = 128;
  aux0 = random(256);
  aux1 = random(256);
  accent = rgb(255, 255, 255);
  for (uint8_t i = 0; i < MAX_RIPPLES; i++) {
    ripples[i].state = 0;
  }
}

void RippleRainbow::frame() {
  if (aux0 == aux1) {
    aux1 = random(256);
  } else if (aux1 > aux0) {
    aux0++;
  } else {
    aux0--;
  }

  Pixel bg = mixPixel(wheel(aux0), rgb(0, 0, 0), 235);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer0[i] = bg;
  }

  drawRipples();
  accent = wheel(aux0);
}

void RippleRainbow::drawRipples() {
  for (uint8_t i = 0; i < MAX_RIPPLES; i++) {
    uint32_t ripplestate = ripples[i].state;

    if (ripplestate) {
      uint32_t rippledecay = (speed >> 4) + 1;
      uint32_t rippleorigin = ripples[i].pos;
      Pixel col = wheel(ripples[i].color);
      uint32_t propagation = ((ripplestate / rippledecay - 1) * (speed + 1));
      int propI = propagation >> 8;
      uint8_t propF = propagation & 0xFF;
      uint8_t amp = (ripplestate < 17)
                      ? triwave8((ripplestate - 1) * 8)
                      : map((long)ripplestate, 17, 255, 255, 2);

      int left = rippleorigin - propI - 1;
      int right = rippleorigin + propI + 2;
      for (int v = 0; v < 4; v++) {
        uint8_t mag = scale8(cubicwave8(propF / 4 + v * 64), amp);
        setLed(left + v, mixPixel(getLed(left + v), col, mag));
        setLed(right - v, mixPixel(getLed(right - v), col, mag));
      }

      ripplestate += rippledecay;
      ripples[i].state = (ripplestate > 254) ? 0 : ripplestate;
    } else {
      if (random(5100 + 10000) <= intensity) {
        ripples[i].state = 1;
        ripples[i].pos = random(NUM_LEDS);
        ripples[i].color = random(256);
      }
    }
  }
}
