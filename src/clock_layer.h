#pragma once

#include "matrix.h"

// Draws HH MM (3x5 digits) in the given color into layer with the given alpha (0-255).
// Draws nothing if time is not synced yet.
void drawClock(Pixel *layer, Pixel color, uint8_t alpha);
