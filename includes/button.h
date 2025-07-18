#pragma once
#include <raylib.h>

#include <string>
#include <vector>

struct Button {
    Rectangle rect;
    std::string label;
    int id;
};

std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset);
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse);