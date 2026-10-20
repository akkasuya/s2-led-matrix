#include <Arduino.h>
#include <WebServer.h>
#include "web_ui.h"
#include "settings.h"
#include "registry.h"
#include "net_time.h"

static WebServer server(80);

static const char PAGE[] = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>LED Matrix</title>
<style>
  :root {
    --bg: #0b0d12;
    --card: #151924;
    --card-2: #1c2231;
    --text: #e8ecf5;
    --muted: #8a94a8;
    --accent: #6d7cff;
    --accent-2: #22d3ee;
    --on: #34d399;
    --off: #3a4256;
    --radius: 16px;
  }
  * { box-sizing: border-box; }
  body {
    margin: 0;
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
    background: radial-gradient(1200px 600px at 10% -10%, #1b2440 0%, var(--bg) 60%) fixed;
    color: var(--text);
    min-height: 100vh;
  }
  .wrap { max-width: 640px; margin: 0 auto; padding: 20px 16px 40px; }
  header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 20px; }
  .brand { display: flex; align-items: center; gap: 12px; }
  .logo {
    width: 40px; height: 40px; border-radius: 12px;
    background: linear-gradient(135deg, var(--accent), var(--accent-2));
    display: grid; place-items: center; font-weight: 700; color: #fff;
  }
  h1 { font-size: 20px; margin: 0; letter-spacing: .2px; }
  .sub { color: var(--muted); font-size: 13px; margin-top: 2px; }
  .card {
    background: linear-gradient(180deg, var(--card), var(--card-2));
    border: 1px solid rgba(255,255,255,.06);
    border-radius: var(--radius);
    padding: 16px;
    margin-bottom: 16px;
    box-shadow: 0 10px 30px rgba(0,0,0,.25);
  }
  .card h2 {
    font-size: 12px; text-transform: uppercase; letter-spacing: 1.2px;
    color: var(--muted); margin: 0 0 12px; font-weight: 600;
  }
  .row {
    display: flex; align-items: center; justify-content: space-between;
    padding: 10px 0; gap: 12px;
  }
  .row + .row { border-top: 1px solid rgba(255,255,255,.05); }
  .label { font-size: 15px; }
  .hint { font-size: 12px; color: var(--muted); margin-top: 2px; }
  .now {
    display: inline-flex; align-items: center; gap: 8px;
    background: rgba(109,124,255,.15); color: #c7ceff;
    padding: 6px 12px; border-radius: 999px; font-size: 13px; font-weight: 600;
  }
  .now::before {
    content: ""; width: 8px; height: 8px; border-radius: 50%;
    background: var(--on); box-shadow: 0 0 10px var(--on);
  }
  .interval { display: flex; flex-direction: column; align-items: stretch; gap: 8px; width: 55%; min-width: 220px; }
  .interval-top { display: flex; align-items: center; justify-content: flex-end; gap: 8px; }
  input[type=number] {
    width: 84px; padding: 8px 10px; border-radius: 10px;
    border: 1px solid rgba(255,255,255,.1); background: #0f131d; color: var(--text);
    font-size: 15px; outline: none;
  }
  input[type=number]:focus { border-color: var(--accent); }
  input[type=range] {
    -webkit-appearance: none; appearance: none; width: 100%; height: 6px;
    border-radius: 999px; background: var(--off); outline: none; margin: 8px 0;
  }
  input[type=range]::-webkit-slider-thumb {
    -webkit-appearance: none; appearance: none; width: 22px; height: 22px;
    border-radius: 50%; background: #fff; cursor: pointer;
    box-shadow: 0 2px 8px rgba(0,0,0,.4);
  }
  input[type=range]::-moz-range-thumb {
    width: 22px; height: 22px; border: 0; border-radius: 50%;
    background: #fff; cursor: pointer; box-shadow: 0 2px 8px rgba(0,0,0,.4);
  }
  input[type=range]::-webkit-slider-runnable-track {
    height: 6px; border-radius: 999px;
    background: linear-gradient(90deg, var(--accent), var(--accent-2));
  }
  input[type=range]::-moz-range-track {
    height: 6px; border-radius: 999px;
    background: linear-gradient(90deg, var(--accent), var(--accent-2));
  }
  input[type=range]:focus-visible::-webkit-slider-thumb { outline: 2px solid var(--accent); outline-offset: 2px; }
  .scale { display: flex; justify-content: space-between; font-size: 11px; color: var(--muted); }
  .alpha { display: flex; align-items: center; gap: 10px; width: 55%; min-width: 220px; }
  .alpha input[type=range] { margin: 0; }
  .alpha #alphaVal { width: 40px; text-align: right; color: var(--text); font-variant-numeric: tabular-nums; }
  .switch { position: relative; width: 50px; height: 28px; flex: none; }
  .switch input { opacity: 0; width: 0; height: 0; position: absolute; }
  .slider {
    position: absolute; inset: 0; border-radius: 999px;
    background: var(--off); transition: background .2s;
    cursor: pointer;
  }
  .slider::before {
    content: ""; position: absolute; width: 22px; height: 22px; left: 3px; top: 3px;
    border-radius: 50%; background: #fff; transition: transform .2s;
    box-shadow: 0 2px 6px rgba(0,0,0,.3);
  }
  .switch input:checked + .slider { background: var(--on); }
  .switch input:checked + .slider::before { transform: translateX(22px); }
  .switch input:focus-visible + .slider { outline: 2px solid var(--accent); outline-offset: 2px; }
  .grid { display: grid; grid-template-columns: 1fr; gap: 8px; }
  @media (min-width: 520px) { .grid { grid-template-columns: 1fr 1fr; } }
  .effect {
    display: flex; align-items: center; justify-content: space-between; gap: 10px;
    padding: 10px 12px; border-radius: 12px;
    background: rgba(255,255,255,.03); border: 1px solid rgba(255,255,255,.05);
    transition: border-color .2s, background .2s;
  }
  .effect.active { border-color: rgba(109,124,255,.6); background: rgba(109,124,255,.10); }
  .effect .name { font-size: 14px; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
  footer { text-align: center; color: var(--muted); font-size: 12px; margin-top: 24px; }
</style>
</head>
<body>
<div class="wrap">
  <header>
    <div class="brand">
      <div class="logo">LM</div>
      <div>
        <h1>LED Matrix</h1>
        <div class="sub">16 × 8 · ESP32-S2</div>
      </div>
    </div>
    <span class="now" id="cur">—</span>
  </header>

  <section class="card">
    <h2>General</h2>
    <div class="row">
      <div>
        <div class="label">Clock</div>
        <div class="hint">Show HH MM overlay</div>
      </div>
      <label class="switch"><input type="checkbox" id="clock"><span class="slider"></span></label>
    </div>
    <div class="row">
      <div>
        <div class="label">Background effects</div>
        <div class="hint">Turn all effects on or off</div>
      </div>
      <label class="switch"><input type="checkbox" id="effects"><span class="slider"></span></label>
    </div>
    <div class="row">
      <div>
        <div class="label">Random order</div>
        <div class="hint">Off = play in list order</div>
      </div>
      <label class="switch"><input type="checkbox" id="random"><span class="slider"></span></label>
    </div>
    <div class="row">
      <div>
        <div class="label">WiFi network</div>
        <div class="hint">Restart into setup to change SSID/password</div>
      </div>
      <button onclick="resetWifi()">Change WiFi</button>
    </div>
    <div class="row">
      <div>
        <div class="label">Clock opacity</div>
        <div class="hint">Blend strength over the effects</div>
      </div>
      <div class="alpha">
        <input type="range" id="alpha" min="0" max="100" step="1">
        <span class="hint" id="alphaVal">70%</span>
      </div>
    </div>
    <div class="row">
      <div>
        <div class="label">Switch interval</div>
        <div class="hint">Seconds between effects (3–3600)</div>
      </div>
      <div class="interval">
        <div class="interval-top">
          <input type="number" id="ival" min="3" max="3600">
          <span class="hint">sec</span>
        </div>
        <input type="range" id="ivalSlider" min="0" max="1000" step="1">
        <div class="scale"><span>3s</span><span>1m</span><span>10m</span><span>1h</span></div>
      </div>
    </div>
  </section>

  <section class="card">
    <h2>Effects</h2>
    <div class="grid" id="list"></div>
  </section>

  <footer>ledmatrix.local</footer>
</div>
<script>
const $ = id => document.getElementById(id);

const MIN_SEC = 3, MAX_SEC = 3600;

// Logarithmic mapping: slider 0..1000 <-> seconds 3..3600
function sliderToSec(p) {
  return Math.round(MIN_SEC * Math.pow(MAX_SEC / MIN_SEC, p / 1000));
}
function secToSlider(v) {
  return Math.round(1000 * Math.log(v / MIN_SEC) / Math.log(MAX_SEC / MIN_SEC));
}

let dragging = false;
let alphaDragging = false;

$('alpha').oninput = () => {
  alphaDragging = true;
  $('alphaVal').textContent = $('alpha').value + '%';
};
$('alpha').onchange = () => {
  alphaDragging = false;
  call('alpha=' + $('alpha').value);
};

function render(s) {
  $('clock').checked = s.clock;
  $('effects').checked = s.effects;
  $('random').checked = s.random;
  if (!alphaDragging) {
    $('alpha').value = s.alpha;
    $('alphaVal').textContent = s.alpha + '%';
  }
  if (!dragging && document.activeElement !== $('ival')) {
    $('ival').value = s.interval;
    $('ivalSlider').value = secToSlider(s.interval);
  }
  $('cur').textContent = s.current >= 0 ? s.list[s.current].name : 'Off';

  const list = $('list');
  list.innerHTML = '';
  s.list.forEach((e, i) => {
    const item = document.createElement('div');
    item.className = 'effect' + (i === s.current ? ' active' : '');

    const name = document.createElement('span');
    name.className = 'name';
    name.textContent = e.name;

    const sw = document.createElement('label');
    sw.className = 'switch';
    const cb = document.createElement('input');
    cb.type = 'checkbox';
    cb.checked = e.on;
    cb.onchange = () => call('effect=' + i + '&on=' + (cb.checked ? 1 : 0));
    const slider = document.createElement('span');
    slider.className = 'slider';
    sw.append(cb, slider);

    item.append(name, sw);
    list.append(item);
  });
}

function call(query) {
  fetch('/api/set?' + query)
    .then(r => r.json())
    .then(render)
    .catch(() => {});
}

function resetWifi() {
  if (!confirm('Forget WiFi and restart into setup mode?')) return;
  fetch('/api/wifi/reset').catch(() => {});
  document.body.innerHTML = '<div class="wrap"><div class="card"><h2>Restarting</h2>' +
    '<div class="hint">Connect to WiFi "LEDMatrix-Setup" (password 55555555) and open http://192.168.4.1 to set the new network.</div></div></div>';
}

function saveInterval(v) {
  call('interval=' + v);
}

$('ivalSlider').oninput = () => {
  dragging = true;
  $('ival').value = sliderToSec(+$('ivalSlider').value);
};
$('ivalSlider').onchange = () => {
  dragging = false;
  saveInterval(sliderToSec(+$('ivalSlider').value));
};
$('ival').onchange = () => {
  const v = Math.min(MAX_SEC, Math.max(MIN_SEC, +$('ival').value || MIN_SEC));
  $('ivalSlider').value = secToSlider(v);
  saveInterval(v);
};

$('clock').onchange = () => call('clock=' + ($('clock').checked ? 1 : 0));
$('effects').onchange = () => call('effects=' + ($('effects').checked ? 1 : 0));
$('random').onchange = () => call('random=' + ($('random').checked ? 1 : 0));

function load() {
  fetch('/api/state').then(r => r.json()).then(render).catch(() => {});
}
load();
setInterval(load, 3000);
</script>
</body>
</html>
)HTML";

static String stateJson() {
  String j = "{";
  j += "\"clock\":" + String(settings.clockOn ? "true" : "false");
  j += ",\"effects\":" + String(settings.effectsOn ? "true" : "false");
  j += ",\"interval\":" + String(settings.switchSec);
  j += ",\"random\":" + String(settings.randomOrder ? "true" : "false");
  j += ",\"alpha\":" + String(settings.clockAlpha);
  j += ",\"current\":" + String(currentEffectIndex);
  j += ",\"list\":[";
  for (uint8_t i = 0; i < EFFECT_COUNT; i++) {
    if (i > 0) j += ",";
    j += "{\"name\":\"" + String(effectList[i].name) + "\",\"on\":";
    j += settings.enabled[i] ? "true" : "false";
    j += "}";
  }
  j += "]}";
  return j;
}

static void handleRoot() {
  server.send(200, "text/html", PAGE);
}

static void handleState() {
  server.send(200, "application/json", stateJson());
}

static void handleWifiReset() {
  server.send(200, "application/json", "{\"ok\":true}");
  delay(300);
  netResetWifi();
}

static void handleSet() {
  if (server.hasArg("clock")) {
    settings.clockOn = server.arg("clock") == "1";
  }
  if (server.hasArg("effects")) {
    settings.effectsOn = server.arg("effects") == "1";
  }
  if (server.hasArg("random")) {
    settings.randomOrder = server.arg("random") == "1";
  }
  if (server.hasArg("alpha")) {
    int v = server.arg("alpha").toInt();
    if (v < 0) v = 0;
    if (v > 100) v = 100;
    settings.clockAlpha = v;
  }
  if (server.hasArg("interval")) {
    int v = server.arg("interval").toInt();
    if (v < SWITCH_MIN_SEC) v = SWITCH_MIN_SEC;
    if (v > SWITCH_MAX_SEC) v = SWITCH_MAX_SEC;
    settings.switchSec = v;
  }
  if (server.hasArg("effect") && server.hasArg("on")) {
    int idx = server.arg("effect").toInt();
    if (idx >= 0 && idx < EFFECT_COUNT) {
      settings.enabled[idx] = server.arg("on") == "1";
    }
  }
  settingsSave();
  server.send(200, "application/json", stateJson());
}

void webBegin() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/state", HTTP_GET, handleState);
  server.on("/api/set", HTTP_GET, handleSet);
  server.on("/api/wifi/reset", HTTP_GET, handleWifiReset);
  server.begin();
}

void webLoop() {
  server.handleClient();
}
