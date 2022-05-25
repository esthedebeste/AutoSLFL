#include "bounds.h"
#include "windows.h"

void Bounds::click_middle() {
  auto mx = x + width / 2;
  auto my = y + height / 2;
  INPUT move{
      .type = INPUT_MOUSE,
      .mi =
          {
              .dx = mx * (65536 / GetSystemMetrics(SM_CXSCREEN)),
              .dy = my * (65536 / GetSystemMetrics(SM_CYSCREEN)),
              .dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE,
          },
  };
  SendInput(1, &move, sizeof(INPUT));
  static INPUT left_down{
      .type = INPUT_MOUSE,
      .mi = {.dwFlags = MOUSEEVENTF_LEFTDOWN},
  };
  static INPUT left_up{
      .type = INPUT_MOUSE,
      .mi = {.dwFlags = MOUSEEVENTF_LEFTUP},
  };
  SendInput(1, &left_down, sizeof(INPUT));
  Sleep(20);
  SendInput(1, &left_up, sizeof(INPUT));
}

// constants for 2560x1440, fullscreen
Bounds item_bounds[4]{
    Bounds(1132, 510, 82, 82),
    Bounds(1251, 510, 82, 82),
    Bounds(1372, 510, 82, 82),
    Bounds(1492, 510, 82, 82),
};
Bounds item_checkmark_bounds[4]{
    Bounds(1158, 471, 29, 29),
    Bounds(1278, 471, 29, 29),
    Bounds(1400, 471, 29, 29),
    Bounds(1522, 471, 29, 29),
};

// clang-format off
Bounds option_bounds[9]{
    Bounds(1026, 741,  146, 108), Bounds(1207, 741,  147, 108), Bounds(1389, 741,  146, 108),
    Bounds(1026, 884,  146, 108), Bounds(1207, 884,  147, 108), Bounds(1389, 884,  146, 108),
    Bounds(1026, 1028, 146, 108), Bounds(1207, 1028, 147, 108), Bounds(1389, 1028, 146, 108),
};
// clang-format on