#pragma once

// WiFi credentials are entered via the setup portal (WiFiManager)
#define WIFI_SETUP_AP     "LEDMatrix-Setup"   // AP name when no WiFi is saved
#define WIFI_SETUP_PASS   "55555555"          // AP password (min 8 chars, empty = open)
#define WIFI_PORTAL_SEC   180                 // portal timeout before restart

#define MDNS_HOSTNAME     "ledmatrix"         // http://ledmatrix.local

#define NTP_SERVER        "pool.ntp.org"
#define TZ_OFFSET_SEC     (7 * 3600)          // UTC+7
#define NTP_RESYNC_MS     (60UL * 60UL * 1000UL)
