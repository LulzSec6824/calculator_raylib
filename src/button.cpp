#include "../includes/button.h"

#include <raylib.h>

#include <string>
#include <vector>

// Creates and returns a vector of Button objects arranged in a calculator
// layout
std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset, const Font& font) {
    // Define button labels and their corresponding IDs in a grid layout
    std::vector<std::vector<std::pair<std::string, int>>> layout = {
        {{"SHIFT", 200}, {"MODE", 201}, {"ON", 202}, {"DEL", 203}, {"AC", 204}},
        {{"sin", 110}, {"cos", 111}, {"tan", 112}, {"log", 113}, {"ln", 114}},
        {{"exp", 115}, {"sqrt", 116}, {"x^y", '^'}, {"(", '('}, {")", ')'}},
        {{"7", '7'}, {"8", '8'}, {"9", '9'}, {"/", '/'}, {"*", '*'}},
        {{"4", '4'}, {"5", '5'}, {"6", '6'}, {"-", '-'}, {"+", '+'}},
        {{"1", '1'}, {"2", '2'}, {"3", '3'}, {"0", '0'}, {".", '.'}},
        {{"+/-", 103}, {"ANS", 205}, {"=", '='}, {"T", 100}, {"C", 101}},
        {{"<-", 102},
         {"hyp", 117},
         {"asin", 118},
         {"acos", 119},
         {"atan", 120}}};

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
            btn.texture  = nullptr;  // Initialize texture pointer to nullptr
            btn.fontSize = 18;
            btn.labelSize =
                MeasureTextEx(font, btn.label.c_str(), btn.fontSize, 0);
            buttons.push_back(btn);
        }
    }
    return buttons;
}

// Draws all calculator buttons and highlights the one under the mouse cursor
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse, const bool isDarkMode) {
    for (const auto& btn : buttons) {
        // Determine button color based on hover state
        Color btnColor =
            CheckCollisionPointRec(mouse, btn.rect) ? SKYBLUE : WHITE;

        DrawRectangleRounded(btn.rect, 0.2f, 0, btnColor);
        DrawRectangleRoundedLines(btn.rect, 0.2f, 0, GRAY);

        // If button has a texture, draw it instead of text
        if (btn.texture != nullptr) {
            // Calculate scaling to fit the texture within the button while
            // maintaining aspect ratio
            float scale = fmin((btn.rect.width - 10) / btn.texture->width,
                               (btn.rect.height - 10) / btn.texture->height);

            // Calculate position to center the texture in the button
            float textureWidth  = btn.texture->width * scale;
            float textureHeight = btn.texture->height * scale;
            Vector2 texturePos  = {
                btn.rect.x + (btn.rect.width - textureWidth) / 2,
                btn.rect.y + (btn.rect.height - textureHeight) / 2};

            // Draw the texture with appropriate tint based on dark mode
            Color tintColor = isDarkMode ? DARKGRAY : WHITE;
            DrawTextureEx(*btn.texture, texturePos, 0.0f, scale, tintColor);
        } else {
            // Draw text label for buttons without texture, using pre-calculated
            // size
            DrawTextEx(font, btn.label.c_str(),
                       {btn.rect.x + (btn.rect.width - btn.labelSize.x) / 2,
                        btn.rect.y + (btn.rect.height - btn.labelSize.y) / 2},
                       btn.fontSize, 0, BLACK);
        }
    }
}