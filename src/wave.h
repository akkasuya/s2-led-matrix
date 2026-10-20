#pragma once

#include <Arduino.h>

// Override the time base used by the beat functions (default: millis())
void setWaveTime(uint32_t t);
void useRealWaveTime();

// Sine wave: theta 0-65535 maps to one full cycle, returns -32767..32767
int16_t sin16(uint16_t theta);

// Beat-synced sine between lowest and highest, bpm88 = BPM * 256
uint16_t beatsin88(uint16_t bpm88, uint16_t lowest, uint16_t highest);

// Beat-synced sawtooth, bpm88 = BPM * 256
uint16_t beat16(uint16_t bpm88);
uint8_t beat8(uint16_t bpm88);

// 8-bit sine/cos: input 0-255 maps to one cycle, output 0-255 (128 = zero crossing)
uint8_t sin8(uint8_t theta);
uint8_t cos8(uint8_t theta);

// Triangle wave 0-255 -> 0-255
uint8_t triwave8(uint8_t in);

// Quadratic ease-in-out wave 0-255 -> 0-255
uint8_t quadwave8(uint8_t in);

// Cubic ease-in-out wave 0-255 -> 0-255
uint8_t cubicwave8(uint8_t in);

// Beat-synced value between lo and hi (BPM as integer)
int beatsinRange(uint8_t bpm, int lo, int hi);
