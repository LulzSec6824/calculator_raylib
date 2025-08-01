#pragma once
#include <cmath>
#include <string>
#include <vector>

#include "../raylib_v5/src/raylib.h"

struct Button {
    Rectangle rect;
    std::string label;
    int id;
    Texture2D* texture;
    Vector2 labelSize;
    int fontSize;
};

std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset, const Font& font);
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse, const bool isDarkMode = false);