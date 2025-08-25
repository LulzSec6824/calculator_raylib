#pragma once
#include "../raylib/src/raylib.h"

struct Theme {
    // Background colors
    Color bgLight;  // White
    Color bgDark;   // Dark Gray

    // Display box colors
    Color displayLight;
    Color displayDark;

    // Text colors
    Color textLight;
    Color textDark;
    Color textHistory;

    // Button colors
    Color numberBgLight;
    Color numberBgDark;
    Color operatorBgLight;
    Color operatorBgDark;
    Color functionBgLight;
    Color functionBgDark;
    Color controlBgLight;
    Color controlBgDark;
    Color specialBgLight;
    Color specialBgDark;
    Color hoverLight;
    Color hoverDark;
    Color btnTextLight;
    Color btnTextDark;

    Theme()
        : bgLight{255, 255, 255, 255},
          bgDark{50, 50, 50, 255},
          displayLight(LIGHTGRAY),
          displayDark(DARKGRAY),
          textLight(BLACK),
          textDark(WHITE),
          textHistory(GRAY),
          numberBgLight{240, 240, 240, 255},
          numberBgDark{60, 60, 60, 255},
          operatorBgLight{230, 230, 250, 255},
          operatorBgDark{70, 70, 90, 255},
          functionBgLight{230, 250, 230, 255},
          functionBgDark{50, 80, 50, 255},
          controlBgLight{250, 230, 230, 255},
          controlBgDark{80, 50, 50, 255},
          specialBgLight{250, 250, 210, 255},
          specialBgDark{90, 90, 40, 255},
          hoverLight{173, 216, 230, 255},
          hoverDark{100, 149, 237, 255},
          btnTextLight(BLACK),
          btnTextDark(WHITE) {}

    // Get background color based on dark mode setting
    Color getBackgroundColor(bool isDarkMode) const noexcept;

    // Get display box color based on dark mode setting
    Color getDisplayColor(bool isDarkMode) const noexcept;

    // Get text color based on dark mode setting
    Color getTextColor(bool isDarkMode) const noexcept;

    // Get faded text color based on dark mode setting
    Color getFadedTextColor(bool isDarkMode) const noexcept;
};