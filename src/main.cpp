#include <raylib.h>

#include <iomanip>
#include <string>
#include <vector>

#include "../includes/button.h"
#include "../includes/calculator.h"

struct BgColor {
    Color white    = {255, 255, 255, 255};
    Color darkGray = {50, 50, 50, 255};
};

int main() {
    // Window and UI layout setup
    int screenWidth  = 350;
    int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "Calculator in RAYLIB");
    SetTargetFPS(60);
    Font font = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    int padding          = 10;
    int buttonRows       = 5;
    int buttonHeight     = 60;
    int buttonSpacing    = 10;
    int displayBoxHeight = 110;
    int bottomPadding    = 10;
    // Calculate top padding to vertically center the calculator
    int topPadding =
        screenHeight -
        (displayBoxHeight + buttonSpacing + buttonRows * buttonHeight +
         (buttonRows - 1) * buttonSpacing) -
        bottomPadding;
    int displayBoxY = topPadding;
    int topOffset   = displayBoxY + displayBoxHeight + buttonSpacing;
    int leftOffset  = padding;
    int btnW        = 75;

    // Calculator state and button setup
    CalculatorState calc;
    std::vector<Button> buttons = CreateButtons(
        btnW, buttonHeight, buttonSpacing, topOffset, leftOffset, font);

    // Update the CE button (ID 100) label to 'T'
    for (auto& btn : buttons) {
        if (btn.id == 100) {  // CE button
            btn.label = "T";
            break;
        }
    }
    // These values are constant, so they can be calculated once outside the
    // loop
    const int displayBoxWidth  = screenWidth - 2 * padding;
    const int displayBoxX      = padding;
    const Rectangle displayBox = {static_cast<float>(displayBoxX),
                                  static_cast<float>(displayBoxY),
                                  static_cast<float>(displayBoxWidth),
                                  static_cast<float>(displayBoxHeight)};

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        int clicked   = -1;
        // Detect button click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (auto& btn : buttons) {
                if (CheckCollisionPointRec(mouse, btn.rect)) {
                    clicked = btn.id;
                    break;
                }
            }
        }
        // Handle button press logic
        if (clicked != -1) {
            HandleButtonPress(calc, clicked);
        }
        BeginDrawing();
        ClearBackground(calc.isDarkMode ? BgColor().white : BgColor().darkGray);

        // Draw display box with theme-appropriate color
        DrawRectangleRec(displayBox, calc.isDarkMode ? LIGHTGRAY : DARKGRAY);

        // Helper function to truncate string to fit width
        auto TruncateToFit = [&](const std::string& text, float fontSize,
                                 float maxWidth) {
            std::string result = text;
            Vector2 size = MeasureTextEx(font, result.c_str(), fontSize, 0);
            while (size.x > maxWidth && result.length() > 1) {
                result.erase(0, 1);
                size = MeasureTextEx(font, result.c_str(), fontSize, 0);
            }
            if (result != text && result.length() > 1) {
                result[0] = '.';
            }
            return result;
        };

        // Calculate text sizes for alignment
        float exprFontSize = 32.0f;
        float dispFontSize = 48.0f;
        float maxTextWidth = displayBox.width - 40;
        std::string exprToDraw =
            TruncateToFit(calc.expression, exprFontSize, maxTextWidth);
        std::string dispToDraw =
            TruncateToFit(calc.display, dispFontSize, maxTextWidth);
        Vector2 exprSize =
            MeasureTextEx(font, exprToDraw.c_str(), exprFontSize, 0);
        Vector2 dispSize =
            MeasureTextEx(font, dispToDraw.c_str(), dispFontSize, 0);

        // Right-align expression and display text within the display box
        float exprX = displayBox.x + displayBox.width - exprSize.x - 20;
        float exprY = displayBox.y + 10;
        float dispX = displayBox.x + displayBox.width - dispSize.x - 20;
        float dispY = displayBox.y + displayBox.height - dispSize.y - 20;

        Color textColor = calc.isDarkMode ? BLACK : WHITE;
        DrawTextEx(font, exprToDraw.c_str(), {exprX, exprY}, exprFontSize, 0,
                   textColor);
        DrawTextEx(font, dispToDraw.c_str(), {dispX, dispY}, dispFontSize, 0,
                   textColor);

        // Draw calculator buttons
        DrawButtons(buttons, font, mouse, calc.isDarkMode);
        EndDrawing();
    }
    // Unload resources
    UnloadFont(font);
    CloseWindow();
    return 0;
}