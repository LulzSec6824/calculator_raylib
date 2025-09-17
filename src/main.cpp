#include <string>
#include <vector>

#include "../includes/button.h"
#include "../includes/calculator.h"
#include "../includes/display.h"
#include "../includes/embedded_resources.h"
#ifndef RELEASE_BUILD
#include "../includes/metrics.h"
#endif
#include "../includes/theme.h"
#include "../raylib/src/raylib.h"

// Forward declarations
std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset, const Font& font);

int main() {
    // Initialize performance metrics (debug builds only)
#ifndef RELEASE_BUILD
    PerformanceMetrics metrics;
#endif

    // UI layout parameters
    const int buttonRows       = 7;
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
    InitWindow(screenWidth, screenHeight, "Scientific Calculator");

// Load embedded resources instead of from files
#ifdef RELEASE_BUILD
    // Use embedded resources in release mode
    Image icon = LoadEmbeddedIcon();
    Font font  = LoadEmbeddedFont();
#else
    // Use file resources in debug mode
    Image icon = LoadImage("resource/calc.png");
    Font font  = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
#endif

    SetWindowIcon(icon);
    SetTargetFPS(50000000);
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

    // Initialize theme
    Theme theme;

    // These values are constant, so they can be calculated once outside the
    // loop
    const int displayBoxWidth  = calculatorWidth;
    const int displayBoxX      = leftOffset;
    const Rectangle displayBox = {static_cast<float>(displayBoxX),
                                  static_cast<float>(displayBoxY),
                                  static_cast<float>(displayBoxWidth),
                                  static_cast<float>(displayBoxHeight)};

    // Initialize display
    Display display(displayBox, font);

    while (!WindowShouldClose()) {
        // Start frame timing for performance metrics (debug builds only)
#ifndef RELEASE_BUILD
        metrics.startFrame();
#endif

        Vector2 mouse = GetMousePosition();
        int clicked   = -1;

        // Only check for button clicks if mouse button is pressed
        // (optimization)
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Detect button click - only check buttons that could be under the
            // mouse
            for (const Button& btn : buttons) {
                if (CheckCollisionPointRec(mouse, btn.rect)) {
                    clicked = btn.id;
                    break;
                }
            }

            // Handle button press logic
            if (clicked != -1) {
                HandleButtonPress(calc, clicked);
            }
        }

        // Get background color for clearing the screen
        Color bgColor = theme.getBackgroundColor(calc.isDarkMode);

        BeginDrawing();
        ClearBackground(bgColor);

        // Draw the calculator display
#ifndef RELEASE_BUILD
        display.draw(calc, theme, metrics.getPerformanceInfo());
#else
        display.draw(calc, theme, "");
#endif

        // Draw calculator buttons
        DrawButtons(buttons, font, mouse, calc.isDarkMode);

        // End frame timing and update metrics (debug builds only)
#ifndef RELEASE_BUILD
        metrics.endFrame();
#endif

        EndDrawing();
    }
    // Unload resources
    UnloadImage(icon);
    UnloadFont(font);
    CloseWindow();
    return 0;
}