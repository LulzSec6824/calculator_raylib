#include <chrono>  // For performance measurement
#include <string>
#include <vector>

#include "../includes/button.h"
#include "../includes/calculator.h"
#include "../includes/embedded_resources.h"
#include "../raylib_v5/src/raylib.h"

// Theme colors for light and dark mode
struct Theme {
    // Background colors
    Color bgLight = {255, 255, 255, 255};  // White
    Color bgDark  = {50, 50, 50, 255};     // Dark Gray

    // Display box colors
    Color displayLight = LIGHTGRAY;
    Color displayDark  = DARKGRAY;

    // Text colors
    Color textLight   = BLACK;
    Color textDark    = WHITE;
    Color textHistory = GRAY;

    // Button colors
    Color buttonNormalLight = RAYWHITE;
    Color buttonNormalDark  = DARKGRAY;
    Color buttonHoverLight  = LIGHTGRAY;
    Color buttonHoverDark   = GRAY;
    Color buttonTextLight   = BLACK;
    Color buttonTextDark    = WHITE;
};

int main() {
    // Performance tracking variables
    double frameTime    = 0.0;
    int frameCount      = 0;
    double avgFrameTime = 0.0;

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
    SetTargetFPS(50000);
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

    // Pre-calculate maximum text width for display
    const float maxTextWidth = displayBox.width - 40;

    // Pre-calculate text positions and sizes
    const float dispFontSize    = 54.0f;
    const float exprFontSize    = 24.0f;
    const float historyFontSize = 20.0f;
    const float statusFontSize  = 16.0f;

    while (!WindowShouldClose()) {
        // Start frame timing
        auto frameStart = std::chrono::high_resolution_clock::now();

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
        // Set theme colors based on the current mode
        Color bgColor = calc.isDarkMode ? theme.bgDark : theme.bgLight;
        Color displayColor =
            calc.isDarkMode ? theme.displayDark : theme.displayLight;
        Color textColor    = calc.isDarkMode ? theme.textDark : theme.textLight;
        Color historyColor = theme.textHistory;
        Color fadedColor   = calc.isDarkMode ? Fade(theme.textDark, 0.5f)
                                             : Fade(theme.textLight, 0.5f);

        BeginDrawing();
        ClearBackground(bgColor);

        // Draw display box with theme-appropriate color
        DrawRectangleRec(displayBox, displayColor);

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

        // Use the expression as the main display, fallback to display string if
        // empty
        std::string mainDisplayString =
            calc.expression.empty() ? calc.display : calc.expression;
        std::string dispToDraw =
            TruncateToFit(mainDisplayString, dispFontSize, maxTextWidth);
        Vector2 dispSize =
            MeasureTextEx(font, dispToDraw.c_str(), dispFontSize, 0);

        // Display history with optimized positioning
        const float historyStartY     = displayBox.y + 10;
        const float historyLineHeight = 25;
        const float historyX          = displayBox.x + 10;

        // Draw history entries
        for (size_t i = 0; i < calc.history.size(); i++) {
            DrawTextEx(font, calc.history[i].c_str(),
                       {historyX, historyStartY + i * historyLineHeight},
                       historyFontSize, 0, historyColor);
        }

        // The top display for the expression is now disabled.

        // Display error message if in error state
        if (calc.errorState && !calc.errorMessage.empty()) {
            std::string errorToDraw = TruncateToFit(
                "Error: " + calc.errorMessage, exprFontSize, maxTextWidth);

            float errorX = displayBox.x + 30;
            float errorY = displayBox.y + displayBox.height - 60;
            DrawTextEx(font, errorToDraw.c_str(), {errorX, errorY},
                       exprFontSize, 0, RED);
        }

        // Calculate display position once per frame
        const float dispX = displayBox.x + displayBox.width - dispSize.x - 30;
        const float dispY = displayBox.y + displayBox.height - dispSize.y - 30;

        // Draw the display text
        DrawTextEx(font, dispToDraw.c_str(), {dispX, dispY}, dispFontSize, 0,
                   textColor);

        // Draw calculator buttons
        DrawButtons(buttons, font, mouse, calc.isDarkMode);

        // Calculate and display performance metrics
        auto frameEnd = std::chrono::high_resolution_clock::now();
        frameTime =
            std::chrono::duration<double, std::milli>(frameEnd - frameStart)
                .count();

        // Update running average
        frameCount++;
        avgFrameTime = avgFrameTime + (frameTime - avgFrameTime) / frameCount;

        // Display performance info
        char perfInfo[64];
        sprintf(perfInfo, "FPS: %d | Frame: %.2f ms | Avg: %.2f ms", GetFPS(),
                frameTime, avgFrameTime);
        DrawTextEx(font, perfInfo,
                   {displayBox.x + 10, displayBox.y + displayBox.height - 30},
                   statusFontSize, 0, fadedColor);

        // Display mode indicator
        const char* modeText = calc.isDarkMode ? "Dark Mode" : "Light Mode";
        DrawTextEx(font, modeText,
                   {displayBox.x + displayBox.width - 100,
                    displayBox.y + displayBox.height - 30},
                   statusFontSize, 0, fadedColor);

        EndDrawing();
    }
    // Unload resources
    UnloadImage(icon);
    UnloadFont(font);
    CloseWindow();
    return 0;
}