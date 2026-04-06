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

int main() {
#ifndef RELEASE_BUILD
    PerformanceMetrics metrics;
#endif

    // UI layout parameters
    constexpr int buttonRows       = 6;
    constexpr int buttonHeight     = 45;
    constexpr int buttonSpacing    = 15;
    constexpr int sidePadding      = buttonSpacing / 2;
    constexpr int displayBoxHeight = 200;
    constexpr int btnW             = 65;

    // Calculate dimensions
    constexpr int calculatorWidth  = 5 * btnW + 4 * buttonSpacing;
    constexpr int calculatorHeight = displayBoxHeight + buttonSpacing + buttonRows * buttonHeight + (buttonRows - 1) * buttonSpacing;
    constexpr int screenWidth      = calculatorWidth + 2 * sidePadding;
    constexpr int screenHeight     = calculatorHeight + 2 * buttonSpacing;

    InitWindow(screenWidth, screenHeight, "Scientific Calculator");

#ifdef RELEASE_BUILD
    Image icon = LoadEmbeddedIcon();
    Font font  = LoadEmbeddedFont();
#else
    Image icon = LoadImage("resource/calc.png");
    Font font  = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, nullptr, 0);
#endif

    SetWindowIcon(icon);
    SetTargetFPS(60);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    // Calculate offsets
    constexpr int topPadding  = buttonSpacing;
    constexpr int leftOffset  = sidePadding;
    constexpr int displayBoxY = topPadding;
    constexpr int topOffset   = displayBoxY + displayBoxHeight + buttonSpacing;

    // Initialize calculator and UI components
    Calculator calc;
    std::vector<Button> buttons = CreateButtons(btnW, buttonHeight, buttonSpacing, topOffset, leftOffset, font);

    constexpr Theme theme{};

    const Rectangle displayBox = {static_cast<float>(leftOffset), static_cast<float>(displayBoxY), static_cast<float>(calculatorWidth),
                                  static_cast<float>(displayBoxHeight)};
    Display display(displayBox, font);

    while (!WindowShouldClose()) {
#ifndef RELEASE_BUILD
        metrics.startFrame();
#endif

        Vector2 mouse = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (const Button &btn : buttons) {
                if (CheckCollisionPointRec(mouse, btn.rect)) {
                    calc.handleButtonPress(btn.id);
                    break;
                }
            }
        }

        Color bgColor = theme.getBackgroundColor(calc.isDarkMode());

        BeginDrawing();
        ClearBackground(bgColor);

#ifndef RELEASE_BUILD
        display.draw(calc, theme, metrics.getPerformanceInfo());
#else
        display.draw(calc, theme, "");
#endif

        DrawButtons(buttons, font, mouse, calc.isDarkMode());

#ifndef RELEASE_BUILD
        metrics.endFrame();
#endif

        EndDrawing();
    }

    UnloadImage(icon);
    UnloadFont(font);
    CloseWindow();
    return 0;
}