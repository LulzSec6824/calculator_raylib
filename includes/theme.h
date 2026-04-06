#pragma once
#include "../raylib/src/raylib.h"

struct Theme {
    // Background colors
    Color bgLight{255, 255, 255, 255};
    Color bgDark{50, 50, 50, 255};

    // Display box colors
    Color displayLight{200, 200, 200, 255}; // LIGHTGRAY equivalent
    Color displayDark{80, 80, 80, 255};     // DARKGRAY equivalent

    // Text colors
    Color textLight{0, 0, 0, 255};         // BLACK
    Color textDark{255, 255, 255, 255};    // WHITE
    Color textHistory{130, 130, 130, 255}; // GRAY

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
    Color btnTextLight{0, 0, 0, 255};
    Color btnTextDark{255, 255, 255, 255};

    // Default constructor
    constexpr Theme() noexcept = default;

    // Get background color based on dark mode setting
    [[nodiscard]] constexpr Color getBackgroundColor(bool isDarkMode) const noexcept { return isDarkMode ? bgDark : bgLight; }

    // Get display box color based on dark mode setting
    [[nodiscard]] constexpr Color getDisplayColor(bool isDarkMode) const noexcept { return isDarkMode ? displayDark : displayLight; }

    // Get text color based on dark mode setting
    [[nodiscard]] constexpr Color getTextColor(bool isDarkMode) const noexcept { return isDarkMode ? textDark : textLight; }

    // Get faded text color based on dark mode setting
    [[nodiscard]] Color getFadedTextColor(bool isDarkMode) const noexcept { return Fade(isDarkMode ? textDark : textLight, 0.5f); }
};