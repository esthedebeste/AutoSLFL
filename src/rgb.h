#pragma once

// typedef required to write ctors because wingdi.h defines a RGB() macro.
typedef struct RGBtag {
  int r;
  int g;
  int b;
  RGBtag(int r, int g, int b) : r(r), g(g), b(b) {}
  RGBtag() {}
} RGB;