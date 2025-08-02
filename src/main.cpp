#include <string>
#include <vector>

#include "../includes/button.h"
#include "../includes/calculator.h"
#include "../raylib_v5/src/raylib.h"
#include "../includes/embedded_resources.h"

struct BgColor {
    Color white    = {255, 255, 255, 255};
    Color darkGray = {50, 50, 50, 255};
};

int main() {
    // UI layout parameters
    const int buttonRows       = 6;
    const int buttonHeight     = 45;
    const int buttonSpacing    = 15;
    const int sidePadding      = buttonSpacing / 2;
    const int displayBoxHeight = 200;
    const int btnW             = 65;

    // Calculate total calculator width and height
    const int calculatorWidth  = 5 * btnW + 4 * buttonSpacing;
    const int calculatorHeight = displayBoxHeight + buttonSpacing +
                                 buttonRows * buttonHeight +
                                 (buttonRows - 1) * buttonSpacing;

    // Window and UI layout setup
    const int screenWidth  = calculatorWidth + 2 * sidePadding;
    const int screenHeight = calculatorHeight + 2 * buttonSpacing;
    InitWindow(screenWidth, screenHeight, "Calculator in RAYLIB");
    
    // Load embedded resources instead of from files
    #ifdef RELEASE_BUILD
        // Use embedded resources in release mode
        Image icon = LoadEmbeddedIcon();
        Font font = LoadEmbeddedFont();
    #else
        // Use file resources in debug mode
        Image icon = LoadImage("resource/calc.png");
        Font font = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
    #endif
    
    SetWindowIcon(icon);
    SetTargetFPS(60);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    // Calculate offsets
    const int topPadding = buttonSpacing;
    const int leftOffset = sidePadding;

    const int displayBoxY = topPadding;
    const int topOffset   = displayBoxY + displayBoxHeight + buttonSpacing;

    // Calculator state and button setup
    CalculatorState calc;
    std::vector<Button> buttons = CreateButtons(
        btnW, buttonHeight, buttonSpacing, topOffset, leftOffset, font);

    // These values are constant, so they can be calculated once outside the
    // loop
    const int displayBoxWidth  = calculatorWidth;
    const int displayBoxX      = leftOffset;
    const Rectangle displayBox = {static_cast<float>(displayBoxX),
                                  static_cast<float>(displayBoxY),
                                  static_cast<float>(displayBoxWidth),
                                  static_cast<float>(displayBoxHeight)};

    // Pre-calculate maximum text width for display
    const float maxTextWidth = displayBox.width - 40;

    // Pre-calculate text positions
    const float dispFontSize = 54.0f;

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        int clicked   = -1;

        // Only check for button clicks if mouse button is pressed
        // (optimization)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Detect button click - only check buttons that could be under the
            // mouse
            for (const auto& btn : buttons) {
                // Quick boundary check before detailed collision check
                if (mouse.x >= btn.rect.x &&
                    mouse.x <= (btn.rect.x + btn.rect.width) &&
                    mouse.y >= btn.rect.y &&
                    mouse.y <= (btn.rect.y + btn.rect.height)) {
                    clicked = btn.id;
                    break;
                }
            }

            // Handle button press logic
            if (clicked != -1) {
                HandleButtonPress(calc, clicked);
            }
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

        // Calculate text sizes for alignment - using pre-calculated constants
        std::string dispToDraw =
            TruncateToFit(calc.display, dispFontSize, maxTextWidth);
        Vector2 dispSize =
            MeasureTextEx(font, dispToDraw.c_str(), dispFontSize, 0);

        // Right-align expression and display text within the display box
        Color textColor = calc.isDarkMode ? BLACK : WHITE;

        // Display history with optimized positioning
        const float historyStartY     = displayBox.y + 10;
        const float historyLineHeight = 25;
        const float historyX          = displayBox.x + 10;
        const float historyFontSize   = 20;

        // Draw history entries
        for (size_t i = 0; i < calc.history.size(); i++) {
            DrawTextEx(font, calc.history[i].c_str(),
                       {historyX, historyStartY + i * historyLineHeight},
                       historyFontSize, 0, GRAY);
        }

        // Calculate display position once per frame
        const float dispX = displayBox.x + displayBox.width - dispSize.x - 30;
        const float dispY = displayBox.y + displayBox.height - dispSize.y - 30;

        // Draw the display text
        DrawTextEx(font, dispToDraw.c_str(), {dispX, dispY}, dispFontSize, 0,
                   textColor);

        // Draw calculator buttons
        DrawButtons(buttons, font, mouse, calc.isDarkMode);
        EndDrawing();
    }
    // Unload resources
    UnloadImage(icon);
    UnloadFont(font);
    CloseWindow();
    return 0;
}