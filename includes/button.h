#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "../raylib/src/raylib.h"

// Button categories for visual styling
enum class ButtonCategory : std::uint8_t { NUMBER, OPERATOR, FUNCTION, CONTROL, SPECIAL };

// Button structure with move semantics
struct Button {
    Rectangle rect;
    std::string label;
    int id;
    Texture2D *texture;
    int fontSize;
    Vector2 labelSize;

    Button(Rectangle r, std::string_view l, int i, const Font &font, Texture2D *tex = nullptr)
        : rect(r), label(l), id(i), texture(tex), fontSize(0), labelSize{} {
        fontSize  = (label.length() > 2) ? 16 : ((label.length() > 1) ? 18 : 22);
        labelSize = MeasureTextEx(font, label.c_str(), static_cast<float>(fontSize), 0.0f);
    }

    // Disable copy (buttons own textures)
    Button(const Button &)            = delete;
    Button &operator=(const Button &) = delete;

    // Enable move
    Button(Button &&) noexcept            = default;
    Button &operator=(Button &&) noexcept = default;

    // Get category for this button
    [[nodiscard]] ButtonCategory getCategory() const noexcept;
};

// Create calculator button layout
[[nodiscard]] std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset, int leftOffset, const Font &font);

// Draw all calculator buttons
void DrawButtons(const std::vector<Button> &buttons, const Font &font, Vector2 mouse, bool isDarkMode = false);
