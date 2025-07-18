#include "../includes/button.h"

#include <raylib.h>

#include <string>
#include <vector>

// Creates and returns a vector of Button objects arranged in a calculator
// layout
std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset) {
    // Define button labels and their corresponding IDs in a grid layout
    std::vector<std::vector<std::pair<std::string, int>>> layout = {
        {{"CE", 100}, {"C", 101}, {"<-", 102}, {"/", '/'}},
        {{"7", '7'}, {"8", '8'}, {"9", '9'}, {"*", '*'}},
        {{"4", '4'}, {"5", '5'}, {"6", '6'}, {"-", '-'}},
        {{"1", '1'}, {"2", '2'}, {"3", '3'}, {"+", '+'}},
        {{"+/-", 103}, {"0", '0'}, {".", '.'}, {"=", '='}}};

    std::vector<Button> buttons;
    for (int row = 0; row < layout.size(); ++row) {
        for (int col = 0; col < layout[row].size(); ++col) {
            Button btn;
            // Set button position and size
            btn.rect  = {static_cast<float>(leftOffset + col * (btnW + margin)),
                         static_cast<float>(topOffset + row * (btnH + margin)),
                         static_cast<float>(btnW), static_cast<float>(btnH)};
            btn.label = layout[row][col].first;
            btn.id    = layout[row][col].second;
            buttons.push_back(btn);
        }
    }
    return buttons;
}

// Draws all calculator buttons and highlights the one under the mouse cursor
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse) {
    for (const auto& btn : buttons) {
        // Highlight button if mouse is hovering
        Color btnColor =
            CheckCollisionPointRec(mouse, btn.rect) ? SKYBLUE : WHITE;
        DrawRectangleRounded(btn.rect, 0.2f, 0, btnColor);
        DrawRectangleRoundedLines(btn.rect, 0.2f, 0, GRAY);
        int fontSize     = 28;
        Vector2 textSize = MeasureTextEx(font, btn.label.c_str(), fontSize, 0);
        // Center the button label
        DrawTextEx(font, btn.label.c_str(),
                   {btn.rect.x + (btn.rect.width - textSize.x) / 2,
                    btn.rect.y + (btn.rect.height - textSize.y) / 2},
                   fontSize, 0, BLACK);
    }
}