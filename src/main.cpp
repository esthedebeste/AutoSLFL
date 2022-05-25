#include "bounds.h"
#include "rgb.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

BYTE saturation(RGBQUAD &rgb) {
  BYTE r = rgb.rgbRed, g = rgb.rgbGreen, b = rgb.rgbBlue;
  BYTE cmax = max(r, max(g, b));
  BYTE cmin = min(r, min(g, b));
  if (cmax == 0)
    return 0;
  return 255 * (cmax - cmin) / cmax;
}

auto avg_nongray(Bounds &bounds) {
  return bounds.get_avg([](RGBQUAD &p) { return saturation(p) >= 40; });
}

void round() {
  RGB opt_avgs[9];
  for (int i = 0; i < 9; i++) {
    auto &bounds = option_bounds[i];
    bounds.take_screenshot();
    RGB avg = avg_nongray(bounds);
    opt_avgs[i] = avg;
  }
  for (int i = 0; i < 4; i++) {
    auto &checkmark = item_checkmark_bounds[i];
    checkmark.take_screenshot();
    RGB checkmark_avg = avg_nongray(checkmark);
    if (checkmark_avg.g > 100)
      // skip if item already completed
      continue;
    auto &bounds = item_bounds[i];
    bounds.take_screenshot();
    auto [avg, confidence] = avg_nongray(bounds);
    if (confidence < 500)
      // skip if item is empty or too small to properly recognize
      continue;
    int closest = -1;
    int closest_dist = INT_MAX;
    for (int j = 0; j < 9; j++) {
      int dist = (avg.r - opt_avgs[j].r) * (avg.r - opt_avgs[j].r) +
                 (avg.g - opt_avgs[j].g) * (avg.g - opt_avgs[j].g) +
                 (avg.b - opt_avgs[j].b) * (avg.b - opt_avgs[j].b);
      if (dist < closest_dist) {
        closest = j;
        closest_dist = dist;
      }
    }
    if (closest == -1) {
      std::cout << "No option found for item " << i << "\n";
      continue;
    }
    std::cout << "Clicking option " << (closest + 1) << " for item " << (i + 1)
              << "\n";
    option_bounds[closest].click_middle();
  }
}

bool looping = false;
std::thread loop;
double seconds = 0.1;
void stop_loop() {
  looping = false;
  if (loop.joinable())
    loop.join();
}
void start_loop() {
  if (looping)
    return;
  looping = true;
  loop = std::thread([]() {
    while (looping) {
      round();
      std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
    }
  });
}

HHOOK hook;

LRESULT key_handler(int code, WPARAM wParam, LPARAM lParam) {
  PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
  if (wParam == WM_KEYDOWN && code == HC_ACTION)
    if (key->vkCode == VK_F2)
      round();
    else if (key->vkCode == VK_F3)
      start_loop();
    else if (key->vkCode == VK_F4)
      stop_loop();
    else if (key->vkCode == VK_F7) {
      if (hook)
        UnhookWindowsHookEx(hook);
      stop_loop();
      exit(0);
    }

  return CallNextHookEx(NULL, code, wParam, lParam);
}

int main(int argv, char **argc) {
  Screenshot s(0, 0, GetSystemMetrics(SM_CXSCREEN),
               GetSystemMetrics(SM_CYSCREEN));
  s.take_screenshot();
  s.save_ppm("test.ppm");
  if (argv > 1)
    seconds = strtod(argc[1], nullptr);
  hook = SetWindowsHookEx(WH_KEYBOARD_LL, key_handler, NULL, 0);
  std::cout << "Press F2 to do one round, F3 to start the "
               "loop ("
            << seconds
            << " seconds per check), F4 to end it. press F7 to exit.\n";

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) != 0) {
  }
  if (hook)
    UnhookWindowsHookEx(hook);
  stop_loop();
  return 0;
}