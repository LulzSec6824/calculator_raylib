#include "../includes/theme.h"

Theme::Theme() {
    // Light theme colors
    numberBgLight   = {240, 240, 240, 255};
    operatorBgLight = {230, 230, 250, 255};
    functionBgLight = {230, 250, 230, 255};
    controlBgLight  = {250, 230, 230, 255};
    specialBgLight  = {250, 250, 210, 255};
    hoverLight      = {173, 216, 230, 255};
    btnTextLight    = BLACK;

    // Dark theme colors
    numberBgDark   = {60, 60, 60, 255};
    operatorBgDark = {70, 70, 90, 255};
    functionBgDark = {50, 80, 50, 255};
    controlBgDark  = {80, 50, 50, 255};
    specialBgDark  = {90, 90, 40, 255};
    hoverDark      = {100, 149, 237, 255};
    btnTextDark    = WHITE;
}

Color Theme::getBackgroundColor(bool isDarkMode) const {
    return isDarkMode ? bgDark : bgLight;
}

Color Theme::getDisplayColor(bool isDarkMode) const {
    return isDarkMode ? displayDark : displayLight;
}

Color Theme::getTextColor(bool isDarkMode) const {
    return isDarkMode ? textDark : textLight;
}

Color Theme::getFadedTextColor(bool isDarkMode) const {
    return isDarkMode ? Fade(textDark, 0.5f) : Fade(textLight, 0.5f);
}