#include "../includes/theme.h"

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