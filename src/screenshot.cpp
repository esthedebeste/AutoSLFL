#pragma once
#include "screenshot.h"
#include <fstream>

Screenshot::Screenshot(int x, int y, int width, int height) {
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  pixels = new RGBQUAD[width * height];
  desktopwnd = GetDesktopWindow();
  desktopDC = GetDC(desktopwnd);
  captureDC = CreateCompatibleDC(desktopDC);
  captureBitmap = CreateCompatibleBitmap(desktopDC, width, height);
  SelectObject(captureDC, captureBitmap);
  rect = {x, y, x + width, y + height};
  SetBoundsRect(captureDC, &rect, DCB_RESET);
  bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
}
Screenshot::~Screenshot() {
  delete[] pixels;
  DeleteDC(captureDC);
  DeleteObject(captureBitmap);
  ReleaseDC(desktopwnd, desktopDC);
}

void Screenshot::take_screenshot() {
  BitBlt(captureDC, 0, 0, width, height, desktopDC, x, y, SRCCOPY | CAPTUREBLT);
  GetDIBits(captureDC, captureBitmap, 0, height, pixels, &bmi, DIB_RGB_COLORS);
}

void Screenshot::save_ppm(std::string to) {
  std::ofstream out(to, std::ios::binary);
  out << "P6\n" << width << " " << height << "\n255\n";
  for (int i = 0; i < width * height; i++)
    out << pixels[i].rgbRed << pixels[i].rgbGreen << pixels[i].rgbBlue;
  out.close();
}

RGBQUAD &Screenshot::at(int x, int y) { return pixels[y * width + x]; }