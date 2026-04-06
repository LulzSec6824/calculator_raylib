#include "../includes/display.h"

#include <string>

Display::Display(Rectangle box, Font displayFont) noexcept : displayBox_(box), font_(displayFont), maxTextWidth_(box.width - 40.0f) {}

void Display::draw(const Calculator &calc, const Theme &theme, std::string_view perfInfo) {
    const bool isDarkMode = calc.isDarkMode();

    // Get theme colors
    const Color displayColor = theme.getDisplayColor(isDarkMode);
    const Color textColor    = theme.getTextColor(isDarkMode);
    const Color historyColor = theme.textHistory;
    const Color fadedColor   = theme.getFadedTextColor(isDarkMode);

    // Draw display box
    DrawRectangleRec(displayBox_, displayColor);

    // Get display strings
    const auto &expression        = calc.getExpression();
    const auto &display           = calc.getDisplay();
    std::string mainDisplayString = expression.empty() ? std::string(display) : std::string(expression);
    std::string dispToDraw        = truncateToFit(mainDisplayString, DISPLAY_FONT_SIZE, maxTextWidth_);
    Vector2 dispSize              = MeasureTextEx(font_, dispToDraw.c_str(), DISPLAY_FONT_SIZE, 0);

    // Draw history
    const float historyStartY = displayBox_.y + 10.0f;
    const float historyX      = displayBox_.x + 10.0f;

    float currentY = historyStartY;
    for (const auto &entry : calc.getHistory()) {
        DrawTextEx(font_, entry.c_str(), Vector2{historyX, currentY}, HISTORY_FONT_SIZE, 0, historyColor);
        currentY += HISTORY_LINE_HEIGHT;
    }

    // Draw error message if present
    if (calc.hasError()) {
        std::string errorText   = "Error: " + std::string(calc.getErrorMessage());
        std::string errorToDraw = truncateToFit(errorText, EXPR_FONT_SIZE, maxTextWidth_);

        float errorX = displayBox_.x + 30.0f;
        float errorY = displayBox_.y + displayBox_.height - 60.0f;
        DrawTextEx(font_, errorToDraw.c_str(), Vector2{errorX, errorY}, EXPR_FONT_SIZE, 0, RED);
    }

    // Draw main display text (right-aligned)
    const float dispX = displayBox_.x + displayBox_.width - dispSize.x - 30.0f;
    const float dispY = displayBox_.y + displayBox_.height - dispSize.y - 30.0f;
    DrawTextEx(font_, dispToDraw.c_str(), Vector2{dispX, dispY}, DISPLAY_FONT_SIZE, 0, textColor);

    // Debug info (non-release builds only)
#ifndef RELEASE_BUILD
    if (!perfInfo.empty()) {
        DrawTextEx(font_, std::string(perfInfo).c_str(), Vector2{displayBox_.x + 10.0f, displayBox_.y + displayBox_.height - 30.0f}, STATUS_FONT_SIZE,
                   0, fadedColor);
    }

    const char *modeText = isDarkMode ? "Dark Mode" : "Light Mode";
    DrawTextEx(font_, modeText, Vector2{displayBox_.x + displayBox_.width - 100.0f, displayBox_.y + displayBox_.height - 30.0f}, STATUS_FONT_SIZE, 0,
               fadedColor);
#else
    (void)perfInfo; // Suppress unused warning
    (void)fadedColor;
#endif
}

std::string Display::truncateToFit(std::string_view text, float fontSize, float maxWidth) const {
    std::string result(text);
    Vector2 size = MeasureTextEx(font_, result.c_str(), fontSize, 0);

    while (size.x > maxWidth && result.length() > 1) {
        result.erase(0, 1);
        size = MeasureTextEx(font_, result.c_str(), fontSize, 0);
    }

    if (result != text && result.length() > 1) {
        result[0] = '.';
    }

    return result;
}