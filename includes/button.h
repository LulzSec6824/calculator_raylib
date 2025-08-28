#pragma once
#include <cmath>
#include <string>
#include <vector>

#include "../raylib/src/raylib.h"

struct Button {
    Rectangle rect;
    std::string label;
    int id;
    Texture2D* texture;
    int fontSize;
    Vector2 labelSize;

    Button(Rectangle r, std::string l, int i, const Font& font,
           Texture2D* tex = nullptr)
        : rect(r),
          label(std::move(l)),
          id(i),
          texture(tex),
          fontSize(0),
          labelSize() {
        fontSize = (label.length() > 2) ? 16 : ((label.length() > 1) ? 18 : 22);
        labelSize = MeasureTextEx(font, label.c_str(),
                                  static_cast<float>(fontSize), 0.0f);
    }

    // Disable copy constructor and assignment operator
    Button(const Button&)            = delete;
    Button& operator=(const Button&) = delete;

    // Enable move constructor and assignment operator
    Button(Button&&)            = default;
    Button& operator=(Button&&) = default;
};

std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset, const Font& font);
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse, bool isDarkMode = false);
