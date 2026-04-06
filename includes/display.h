#pragma once

#include <string>
#include <string_view>

#include "../raylib/src/raylib.h"
#include "calculator.h"
#include "theme.h"

class Display {
  public:
    Display(Rectangle box, Font displayFont) noexcept;

    // Disable copy
    Display(const Display &)            = delete;
    Display &operator=(const Display &) = delete;

    // Enable move
    Display(Display &&) noexcept            = default;
    Display &operator=(Display &&) noexcept = default;

    // Draw the calculator display with all elements
    void draw(const Calculator &calc, const Theme &theme, std::string_view perfInfo);

    // Accessors
    [[nodiscard]] Rectangle getDisplayBox() const noexcept { return displayBox_; }
    [[nodiscard]] float getMaxTextWidth() const noexcept { return maxTextWidth_; }

  private:
    Rectangle displayBox_;
    Font font_;
    float maxTextWidth_;

    // Font sizes (constexpr where possible)
    static constexpr float DISPLAY_FONT_SIZE   = 54.0f;
    static constexpr float EXPR_FONT_SIZE      = 24.0f;
    static constexpr float HISTORY_FONT_SIZE   = 20.0f;
    static constexpr float STATUS_FONT_SIZE    = 16.0f;
    static constexpr float HISTORY_LINE_HEIGHT = 25.0f;

    // Helper function to truncate string to fit width
    [[nodiscard]] std::string truncateToFit(std::string_view text, float fontSize, float maxWidth) const;
};