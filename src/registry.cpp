#include "registry.h"
#include "effects/digital_rain.h"
#include "effects/pride2015.h"
#include "effects/gradient.h"
#include "effects/sinelon.h"
#include "effects/plasma.h"
#include "effects/ripple_rainbow.h"
#include "effects/pacifica.h"
#include "effects/candle_multi.h"
#include "effects/solid_glitter.h"
#include "effects/dynamic_smooth.h"
#include "effects/blends.h"
#include "effects/chunchun.h"
#include "effects/sine.h"
#include "effects/twinkleup.h"
#include "effects/drip.h"
#include "effects/popcorn.h"

// Add new effects here (and bump MAX_EFFECTS in registry.h if needed)
static DigitalRain digitalRain;
static Pride2015 pride2015;
static Gradient gradient;
static Sinelon sinelon;
static Plasma plasma;
static RippleRainbow rippleRainbow;
static Pacifica pacifica;
static CandleMulti candleMulti;
static SolidGlitter solidGlitter;
static DynamicSmooth dynamicSmooth;
static Blends blends;
static Chunchun chunchun;
static Sine sine;
static TwinkleUp twinkleUp;
static Drip drip;
static Popcorn popcorn;

EffectEntry effectList[] = {
  {"Digital Rain", &digitalRain},
  {"Pride 2015", &pride2015},
  {"Gradient", &gradient},
  {"Sinelon", &sinelon},
  {"Plasma", &plasma},
  {"Ripple Rainbow", &rippleRainbow},
  {"Pacifica", &pacifica},
  {"Candle Multi", &candleMulti},
  {"Solid Glitter", &solidGlitter},
  {"Dynamic Smooth", &dynamicSmooth},
  {"Blends", &blends},
  {"Chunchun", &chunchun},
  {"Sine", &sine},
  {"TwinkleUp", &twinkleUp},
  {"Drip", &drip},
  {"Popcorn", &popcorn},
};

const uint8_t EFFECT_COUNT = sizeof(effectList) / sizeof(effectList[0]);

static_assert(sizeof(effectList) / sizeof(effectList[0]) <= MAX_EFFECTS,
              "Increase MAX_EFFECTS in registry.h");
