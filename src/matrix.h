#pragma once

#include <Adafruit_NeoPixel.h>

#define LED_PIN     16
#define COLS        16
#define ROWS        8
#define NUM_LEDS    (COLS * ROWS)
#define BRIGHTNESS  40

// a = 255 opaque, 0 transparent
struct Pixel {
  uint8_t r, g, b, a;
};

inline Pixel rgb(uint8_t r, uint8_t g, uint8_t b) {
  return Pixel{r, g, b, 255};
}

extern Adafruit_NeoPixel strip;
extern Pixel layer0[NUM_LEDS];
extern Pixel layer1[NUM_LEDS];

// Column c, row r (r = 0 is bottom). LED index = c * 8 + r
inline uint16_t xyToIndex(uint8_t col, uint8_t row) {
  return col * ROWS + row;
}

void clearLayer(Pixel *layer);
void putPixel(Pixel *layer, uint8_t col, uint8_t row, Pixel color);
void compositeLayers();   // blend layer1 over layer0 into strip buffer
