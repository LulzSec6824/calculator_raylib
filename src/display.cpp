#include "../includes/display.h"

Display::Display(Rectangle box, Font displayFont) : displayBox(box), font(displayFont), maxTextWidth(box.width - 40) {}

void Display::draw(const CalculatorState& calc, const Theme& theme, const std::string& perfInfo) {
    // Get theme colors based on current mode
    Color displayColor = theme.getDisplayColor(calc.isDarkMode);
    Color textColor    = theme.getTextColor(calc.isDarkMode);
    Color historyColor = theme.textHistory;
    Color fadedColor   = theme.getFadedTextColor(calc.isDarkMode);

    // Draw display box with theme-appropriate color
    DrawRectangleRec(displayBox, displayColor);

    // Use the expression as the main display, fallback to display string if
    // empty
    std::string mainDisplayString = calc.expression.empty() ? calc.display : calc.expression;
    std::string dispToDraw        = truncateToFit(mainDisplayString, dispFontSize, maxTextWidth);
    Vector2 dispSize              = MeasureTextEx(font, dispToDraw.c_str(), dispFontSize, 0);

    // Display history with optimized positioning
    const float historyStartY     = displayBox.y + 10.0f;
    const float historyLineHeight = 25.0f;
    const float historyX          = displayBox.x + 10.0f;

    // Draw history entries
    float currentY = historyStartY;
    for (std::vector<std::string>::const_iterator it = calc.history.begin(); it != calc.history.end(); ++it) {
        DrawTextEx(font, it->c_str(), Vector2{historyX, currentY}, historyFontSize, 0, historyColor);
        currentY += historyLineHeight;
    }

    // Display error message if in error state
    if (calc.errorState && !calc.errorMessage.empty()) {
        std::string errorToDraw = truncateToFit("Error: " + calc.errorMessage, exprFontSize, maxTextWidth);

        float errorX = displayBox.x + 30.0f;
        float errorY = displayBox.y + displayBox.height - 60.0f;
        DrawTextEx(font, errorToDraw.c_str(), Vector2{errorX, errorY}, exprFontSize, 0, RED);
    }

    // Calculate display position
    const float dispX = displayBox.x + displayBox.width - dispSize.x - 30.0f;
    const float dispY = displayBox.y + displayBox.height - dispSize.y - 30.0f;

    // Draw the display text
    DrawTextEx(font, dispToDraw.c_str(), Vector2{dispX, dispY}, dispFontSize, 0, textColor);

    // Display performance info only in debug builds
#ifndef RELEASE_BUILD
    DrawTextEx(font, perfInfo.c_str(), Vector2{displayBox.x + 10.0f, displayBox.y + displayBox.height - 30.0f}, statusFontSize, 0, fadedColor);

    // Display mode indicator
    const char* modeText = calc.isDarkMode ? "Dark Mode" : "Light Mode";
    DrawTextEx(font, modeText, Vector2{displayBox.x + displayBox.width - 100.0f, displayBox.y + displayBox.height - 30.0f}, statusFontSize, 0,
               fadedColor);
#endif
}

std::string Display::truncateToFit(const std::string& text, float fontSize, float maxWidth) const {
    std::string result = text;
    Vector2 size       = MeasureTextEx(font, result.c_str(), fontSize, 0);
    while (size.x > maxWidth && result.length() > 1) {
        result.erase(0, 1);
        size = MeasureTextEx(font, result.c_str(), fontSize, 0);
    }
    if (result != text && result.length() > 1) {
        result[0] = '.';
    }
    return result;
}