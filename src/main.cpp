#include <raylib.h>

#include <iomanip>
#include <string>
#include <vector>

#include "../includes/button.h"
#include "../includes/calculator.h"

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
    std::vector<Button> buttons =
        CreateButtons(btnW, buttonHeight, buttonSpacing, topOffset, leftOffset);
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
        ClearBackground(DARKGRAY);

        int displayBoxWidth  = screenWidth - 2 * padding;
        int displayBoxX      = padding;
        Rectangle displayBox = {static_cast<float>(displayBoxX),
                                static_cast<float>(displayBoxY),
                                static_cast<float>(displayBoxWidth),
                                static_cast<float>(displayBoxHeight)};

        // Helper function to truncate string to fit width
        auto TruncateToFit = [&](const std::string& text, float fontSize,
                                 float maxWidth) {
            std::string result = text;
            while (MeasureTextEx(font, result.c_str(), fontSize, 0).x >
                       maxWidth &&
                   result.length() > 1) {
                result.erase(0, 1);
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

        DrawTextEx(font, exprToDraw.c_str(), {exprX, exprY}, exprFontSize, 0,
                   WHITE);
        DrawTextEx(font, dispToDraw.c_str(), {dispX, dispY}, dispFontSize, 0,
                   WHITE);

        // Draw calculator buttons
        DrawButtons(buttons, font, mouse);
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}