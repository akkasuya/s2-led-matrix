#include <Arduino.h>
#include "matrix.h"
#include "net_time.h"
#include "clock_layer.h"
#include "registry.h"
#include "settings.h"
#include "web_ui.h"

int currentEffectIndex = -1;
static unsigned long lastSwitchMs = 0;

static int pickRandomEffect() {
  int enabledCount = 0;
  for (uint8_t i = 0; i < EFFECT_COUNT; i++) {
    if (settings.enabled[i]) enabledCount++;
  }
  if (enabledCount == 0) return -1;

  int pick = -1;
  do {
    int r = random(enabledCount);
    for (uint8_t i = 0; i < EFFECT_COUNT; i++) {
      if (!settings.enabled[i]) continue;
      if (r == 0) {
        pick = i;
        break;
      }
      r--;
    }
  } while (enabledCount > 1 && pick == currentEffectIndex);
  return pick;
}

static int pickNextEffect() {
  int start = currentEffectIndex;
  for (int k = 1; k <= EFFECT_COUNT; k++) {
    int idx = (start + k) % EFFECT_COUNT;
    if (settings.enabled[idx]) return idx;
  }
  return -1;
}

static void switchEffect() {
  int next = settings.randomOrder ? pickRandomEffect() : pickNextEffect();
  currentEffectIndex = next;
  if (next >= 0) {
    effectList[next].fx->begin();
    lastSwitchMs = millis();
  }
}

// Setup mode: blink yellow on LED 0
static void showSetupIndicator() {
  bool on = (millis() / 500) % 2 == 0;
  strip.clear();
  if (on) strip.setPixelColor(0, strip.Color(255, 200, 0));
  strip.show();
}

// BOOT button (GPIO0): hold for CLEAR_HOLD_MS to forget WiFi and restart into setup mode.
// LED 0 turns red while the button is held. Returns true while the button is held.
static const uint8_t BOOT_BUTTON_PIN = 0;
static const unsigned long CLEAR_HOLD_MS = 3000;
static unsigned long bootPressStart = 0;

static bool bootButtonHeld() {
  if (digitalRead(BOOT_BUTTON_PIN) != LOW) {
    bootPressStart = 0;
    return false;
  }
  if (bootPressStart == 0) bootPressStart = millis();

  strip.clear();
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.show();

  if (millis() - bootPressStart >= CLEAR_HOLD_MS) {
    netResetWifi();
  }
  return true;
}

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.clear();
  strip.show();

  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  settingsLoad();
  netBegin();
  webBegin();
  switchEffect();
}

void loop() {
  netUpdate();
  webLoop();

  if (bootButtonHeld()) {
    delay(20);
    return;
  }

  if (netSetupMode()) {
    showSetupIndicator();
    delay(20);
    return;
  }

  bool intervalElapsed = millis() - lastSwitchMs >= settings.switchSec * 1000UL;
  if (currentEffectIndex < 0 || !settings.enabled[currentEffectIndex] || intervalElapsed) {
    switchEffect();
  }

  Effect *fx = (currentEffectIndex >= 0) ? effectList[currentEffectIndex].fx : nullptr;

  clearLayer(layer0);
  if (fx && settings.effectsOn) fx->frame();

  clearLayer(layer1);
  if (settings.clockOn) {
    uint8_t alpha = settings.clockAlpha * 255 / 100;
    drawClock(layer1, fx ? fx->accentColor() : rgb(255, 255, 255), alpha);
  }

  compositeLayers();
  strip.show();

  delay(fx ? fx->frameMs() : 20);
}
