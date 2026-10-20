#pragma once

void netBegin();      // start WiFi; if no connection, opens the setup portal (non-blocking)
void netUpdate();     // run the portal, or when WiFi is up: start mDNS, sync NTP, resync every NTP_RESYNC_MS
bool netSetupMode();  // true while the setup portal is open
void netResetWifi();  // forget saved WiFi credentials and restart into the setup portal
