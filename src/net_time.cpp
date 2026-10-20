#include <Arduino.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <time.h>
#include "config.h"
#include "net_time.h"

static bool syncRequested = false;
static unsigned long lastSyncMs = 0;
static bool mdnsStarted = false;

static WiFiManager wm;
static bool setupActive = false;
static unsigned long setupStartMs = 0;

void netBegin() {
  wm.setConfigPortalBlocking(false);
  if (!wm.autoConnect(WIFI_SETUP_AP, WIFI_SETUP_PASS)) {
    setupActive = true;
    setupStartMs = millis();
  }
}

bool netSetupMode() {
  return setupActive;
}

void netResetWifi() {
  WiFiManager reset;
  reset.resetSettings();
  ESP.restart();
}

void netUpdate() {
  if (WiFi.status() != WL_CONNECTED) {
    if (setupActive) {
      wm.process();
      if (millis() - setupStartMs > WIFI_PORTAL_SEC * 1000UL) {
        ESP.restart();
      }
    }
    return;
  }
  setupActive = false;

  if (!mdnsStarted) {
    mdnsStarted = MDNS.begin(MDNS_HOSTNAME);
    if (mdnsStarted) MDNS.addService("http", "tcp", 80);
  }

  if (syncRequested && millis() - lastSyncMs < NTP_RESYNC_MS) return;

  syncRequested = true;
  lastSyncMs = millis();
  configTime(TZ_OFFSET_SEC, 0, NTP_SERVER);
}
