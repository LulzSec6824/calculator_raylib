#include <raylib.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "../includes/button.h"
#include "../includes/calculator.h"

int main() {
    int screenWidth  = 350;
    int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "Calculator in RAYLIB");
    SetTargetFPS(60);
    Font font = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    CalculatorState calc;
    int btnW = 75, btnH = 60, margin = 10, topOffset = 120, leftOffset = 10;
    std::vector<Button> buttons =
        CreateButtons(btnW, btnH, margin, topOffset, leftOffset);
    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        int clicked   = -1;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (auto& btn : buttons) {
                if (CheckCollisionPointRec(mouse, btn.rect)) {
                    clicked = btn.id;
                    break;
                }
            }
        }
        if (clicked != -1) {
            HandleButtonPress(calc, clicked);
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Define layout variables if not already defined
        int padding       = 24;
        int buttonRows    = 5;
        int buttonHeight  = 64;
        int buttonSpacing = 12;

        // Calculate display box position and size
        int displayBoxWidth  = screenWidth - 2 * padding;
        int displayBoxHeight = 110;
        int displayBoxX      = padding;
        int displayBoxY =
            (screenHeight - (displayBoxHeight +
                             (buttonRows * (buttonHeight + buttonSpacing)))) /
            2;
        Rectangle displayBox = {(float)displayBoxX, (float)displayBoxY,
                                (float)displayBoxWidth,
                                (float)displayBoxHeight};

        // Draw display box (single, main background)
        DrawRectangleRounded(displayBox, 0.1f, 16, LIGHTGRAY);
        DrawRectangleRoundedLines(displayBox, 0.1f, 16, GRAY);

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
                   DARKGRAY);
        DrawTextEx(font, dispToDraw.c_str(), {dispX, dispY}, dispFontSize, 0,
                   BLACK);

        // Draw buttons
        DrawButtons(buttons, font, mouse);
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}