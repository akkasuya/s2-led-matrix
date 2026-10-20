#include <time.h>
#include "clock_layer.h"

// 3x5 digit font, one byte per row, bit 2 = left column, bit 0 = right column
static const uint8_t DIGITS[10][5] = {
  {0b111, 0b101, 0b101, 0b101, 0b111},  // 0
  {0b010, 0b110, 0b010, 0b010, 0b111},  // 1
  {0b111, 0b001, 0b111, 0b100, 0b111},  // 2
  {0b111, 0b001, 0b111, 0b001, 0b111},  // 3
  {0b101, 0b101, 0b111, 0b001, 0b001},  // 4
  {0b111, 0b100, 0b111, 0b001, 0b111},  // 5
  {0b111, 0b100, 0b111, 0b101, 0b111},  // 6
  {0b111, 0b001, 0b001, 0b001, 0b001},  // 7
  {0b111, 0b101, 0b111, 0b101, 0b111},  // 8
  {0b111, 0b101, 0b111, 0b001, 0b111},  // 9
};

// Layout: HH at cols 0-6, gap cols 7-8, MM at cols 9-15. Glyph rows 1-5.
static const uint8_t GLYPH_ROW = 1;

static void drawDigit(Pixel *layer, uint8_t x, uint8_t digit, Pixel color) {
  for (uint8_t fr = 0; fr < 5; fr++) {
    for (uint8_t fc = 0; fc < 3; fc++) {
      if (DIGITS[digit][fr] & (0b100 >> fc)) {
        putPixel(layer, x + fc, GLYPH_ROW + (4 - fr), color);
      }
    }
  }
}

void drawClock(Pixel *layer, Pixel color, uint8_t alpha) {
  struct tm t;
  if (!getLocalTime(&t, 0)) return;

  color.a = alpha;
  drawDigit(layer, 0, t.tm_hour / 10, color);
  drawDigit(layer, 4, t.tm_hour % 10, color);
  drawDigit(layer, 9, t.tm_min / 10, color);
  drawDigit(layer, 13, t.tm_min % 10, color);
}
