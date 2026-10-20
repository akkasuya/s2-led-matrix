#pragma once

#include "matrix.h"

// Rainbow color for pos 0-255
Pixel wheel(uint8_t pos);

// a * (255 - amount) + b * amount, per channel
Pixel blendPixel(Pixel a, Pixel b, uint8_t amount);

// Saturating per-channel add
Pixel addPixel(Pixel a, Pixel b);

uint8_t scale8(uint8_t a, uint8_t b);
uint16_t scale16(uint16_t a, uint16_t b);
uint8_t qadd8(uint8_t a, uint8_t b);
uint8_t qsub8(uint8_t a, uint8_t b);
