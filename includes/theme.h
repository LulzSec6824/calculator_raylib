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
    Color numberBgLight, numberBgDark;
    Color operatorBgLight, operatorBgDark;
    Color functionBgLight, functionBgDark;
    Color controlBgLight, controlBgDark;
    Color specialBgLight, specialBgDark;
    Color hoverLight, hoverDark;
    Color btnTextLight, btnTextDark;

    Theme();

    // Get background color based on dark mode setting
    Color getBackgroundColor(bool isDarkMode) const;

    // Get display box color based on dark mode setting
    Color getDisplayColor(bool isDarkMode) const;

    // Get text color based on dark mode setting
    Color getTextColor(bool isDarkMode) const;

    // Get faded text color based on dark mode setting
    Color getFadedTextColor(bool isDarkMode) const;
};