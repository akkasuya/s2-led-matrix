#include "digital_rain.h"
#include "../matrix.h"

void DigitalRain::respawn(Drop &d) {
  d.len = random(3, 9);
  d.head = ROWS - 1 + random(0, ROWS);
  d.wait = random(0, 15);
}

void DigitalRain::begin() {
  for (uint8_t c = 0; c < COLS; c++) {
    respawn(drops[c]);
    drops[c].head = random(-4, ROWS + 4);
  }
}

void DigitalRain::frame() {
  for (uint8_t c = 0; c < COLS; c++) {
    Drop &d = drops[c];

    if (d.wait > 0) {
      d.wait--;
      continue;
    }

    for (uint8_t k = 0; k < d.len; k++) {
      int r = d.head + k;
      if (r < 0 || r >= ROWS) continue;

      if (k == 0) {
        putPixel(layer0, c, r, rgb(180, 255, 180));
      } else {
        uint8_t g = 220 * (d.len - k) / d.len;
        putPixel(layer0, c, r, rgb(0, g, 0));
      }
    }

    d.head--;
    if (d.head + d.len - 1 < 0) {
      respawn(d);
    }
  }
}
