#pragma once

#include "../effect.h"

class Pacifica : public Effect {
public:
  void begin() override;
  void frame() override;
  uint16_t frameMs() override { return 20; }
  Pixel accentColor() override { return accent; }

private:
  uint16_t ci[4];   // color index start for each of the four wave layers
  Pixel accent;
};
