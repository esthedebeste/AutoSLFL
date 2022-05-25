#pragma once
#include "rgb.h"
#include "screenshot.h"
#include <array>

struct GetAvgResult {
  RGB avg;
  int pixelcount;
  operator RGBtag() { return avg; }
};
struct Bounds : Screenshot {
  using Screenshot::Screenshot;
  template <typename F> GetAvgResult get_avg(F filter) {
    int r = 0, g = 0, b = 0, total = 0;
    for (int i = 0; i < width * height; i++) {
      auto &p = pixels[i];
      if (filter(p)) {
        r += p.rgbRed;
        g += p.rgbGreen;
        b += p.rgbBlue;
        total++;
      }
    }
    if (total == 0)
      return {{0, 0, 0}, 0};
    return {{r / total, g / total, b / total}, total};
  }
  void click_middle();
};

extern Bounds item_bounds[4];
extern Bounds item_checkmark_bounds[4];
extern Bounds option_bounds[9];