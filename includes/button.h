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
    Vector2 labelSize;
    int fontSize;

    Button(Rectangle r, std::string l, int i, const Font& font,
           Texture2D* tex = nullptr)
        : rect(r), label(std::move(l)), id(i), texture(tex) {
        fontSize  = label.length() > 2 ? 16 : (label.length() > 1 ? 18 : 22);
        labelSize = MeasureTextEx(font, label.c_str(),
                                  static_cast<float>(fontSize), 0.0f);
    }
};

std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset, const Font& font);
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse, const bool isDarkMode = false);