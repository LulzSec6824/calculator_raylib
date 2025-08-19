#pragma once
#include "../raylib/src/raylib.h"

struct Theme {
    // Background colors
    Color bgLight = {255, 255, 255, 255};  // White
    Color bgDark  = {50, 50, 50, 255};     // Dark Gray

    // Display box colors
    Color displayLight = LIGHTGRAY;
    Color displayDark  = DARKGRAY;

    // Text colors
    Color textLight   = BLACK;
    Color textDark    = WHITE;
    Color textHistory = GRAY;

    // Button colors
    Color numberBgLight{240, 240, 240, 255};
    Color numberBgDark{60, 60, 60, 255};
    Color operatorBgLight{230, 230, 250, 255};
    Color operatorBgDark{70, 70, 90, 255};
    Color functionBgLight{230, 250, 230, 255};
    Color functionBgDark{50, 80, 50, 255};
    Color controlBgLight{250, 230, 230, 255};
    Color controlBgDark{80, 50, 50, 255};
    Color specialBgLight{250, 250, 210, 255};
    Color specialBgDark{90, 90, 40, 255};
    Color hoverLight{173, 216, 230, 255};
    Color hoverDark{100, 149, 237, 255};
    Color btnTextLight = BLACK;
    Color btnTextDark  = WHITE;

    // Get background color based on dark mode setting
    Color getBackgroundColor(bool isDarkMode) const;

    // Get display box color based on dark mode setting
    Color getDisplayColor(bool isDarkMode) const;

    // Get text color based on dark mode setting
    Color getTextColor(bool isDarkMode) const;

    // Get faded text color based on dark mode setting
    Color getFadedTextColor(bool isDarkMode) const;
};