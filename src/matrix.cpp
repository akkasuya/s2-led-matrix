#include "matrix.h"

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Pixel layer0[NUM_LEDS];
Pixel layer1[NUM_LEDS];

void clearLayer(Pixel *layer) {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    layer[i] = Pixel{0, 0, 0, 0};
  }
}

void putPixel(Pixel *layer, uint8_t col, uint8_t row, Pixel color) {
  if (col >= COLS || row >= ROWS) return;
  layer[xyToIndex(col, row)] = color;
}

void compositeLayers() {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    const Pixel &bg = layer0[i];
    const Pixel &fg = layer1[i];
    uint16_t a = fg.a;
    uint16_t ia = 255 - a;
    uint8_t r = (fg.r * a + bg.r * ia) / 255;
    uint8_t g = (fg.g * a + bg.g * ia) / 255;
    uint8_t b = (fg.b * a + bg.b * ia) / 255;
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}
