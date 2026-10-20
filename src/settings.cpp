#include <Preferences.h>
#include "settings.h"

Settings settings;

static Preferences prefs;

void settingsLoad() {
  settings.clockOn = true;
  settings.effectsOn = true;
  settings.switchSec = 15;
  settings.randomOrder = false;
  settings.clockAlpha = 70;
  for (uint8_t i = 0; i < MAX_EFFECTS; i++) {
    settings.enabled[i] = true;
  }

  prefs.begin("s2", true);
  settings.clockOn = prefs.getBool("clock", settings.clockOn);
  settings.effectsOn = prefs.getBool("effects", settings.effectsOn);
  settings.switchSec = prefs.getUShort("interval", settings.switchSec);
  settings.randomOrder = prefs.getBool("random", settings.randomOrder);
  settings.clockAlpha = prefs.getUChar("alpha", settings.clockAlpha);
  for (uint8_t i = 0; i < EFFECT_COUNT; i++) {
    char key[8];
    snprintf(key, sizeof(key), "e%d", i);
    settings.enabled[i] = prefs.getBool(key, settings.enabled[i]);
  }
  prefs.end();

  if (settings.switchSec < SWITCH_MIN_SEC) settings.switchSec = SWITCH_MIN_SEC;
  if (settings.switchSec > SWITCH_MAX_SEC) settings.switchSec = SWITCH_MAX_SEC;
  if (settings.clockAlpha > 100) settings.clockAlpha = 100;
}

void settingsSave() {
  prefs.begin("s2", false);
  prefs.putBool("clock", settings.clockOn);
  prefs.putBool("effects", settings.effectsOn);
  prefs.putUShort("interval", settings.switchSec);
  prefs.putBool("random", settings.randomOrder);
  prefs.putUChar("alpha", settings.clockAlpha);
  for (uint8_t i = 0; i < EFFECT_COUNT; i++) {
    char key[8];
    snprintf(key, sizeof(key), "e%d", i);
    prefs.putBool(key, settings.enabled[i]);
  }
  prefs.end();
}
