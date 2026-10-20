#pragma once

#include "effect.h"

#define MAX_EFFECTS 16

struct EffectEntry {
  const char *name;
  Effect *fx;
};

extern EffectEntry effectList[];
extern const uint8_t EFFECT_COUNT;

// Index of the effect currently shown, -1 if none is enabled (defined in main.cpp)
extern int currentEffectIndex;
