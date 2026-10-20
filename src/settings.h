#pragma once

#include "registry.h"

#define SWITCH_MIN_SEC  3
#define SWITCH_MAX_SEC  3600

struct Settings {
  bool clockOn;
  bool effectsOn;
  uint16_t switchSec;
  bool randomOrder;
  uint8_t clockAlpha;    // clock layer opacity in percent 0-100
  bool enabled[MAX_EFFECTS];
};

extern Settings settings;

void settingsLoad();
void settingsSave();
