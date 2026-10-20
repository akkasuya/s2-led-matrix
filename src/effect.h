#pragma once

#include <Arduino.h>
#include "matrix.h"

class Effect {
public:
  virtual ~Effect() {}
  virtual void begin() = 0;
  virtual void frame() = 0;            // draw one frame into layer0
  virtual uint16_t frameMs() = 0;
  virtual Pixel accentColor() = 0;     // color used for the clock font
};
