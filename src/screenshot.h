#pragma once
#include "windows.h"
#include "wingdi.h"
#include <string>

class Screenshot {
public:
  int x;
  int y;
  int width;
  int height;
  RGBQUAD *pixels;
  RECT rect;
  HWND desktopwnd;
  HDC desktopDC;
  HDC captureDC;
  HBITMAP captureBitmap;
  BITMAPINFO bmi;

  Screenshot(int x, int y, int width, int height);
  ~Screenshot();
  void take_screenshot();
  RGBQUAD &at(int x, int y);
  void save_ppm(std::string to);
};
