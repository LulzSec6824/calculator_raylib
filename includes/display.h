#pragma once
#include <string>

#include "../raylib/src/raylib.h"
#include "calculator.h"
#include "theme.h"

class Display {
   private:
    Rectangle displayBox;
    Font font;
    float maxTextWidth;
    float dispFontSize;
    float exprFontSize;
    float historyFontSize;
    float statusFontSize;

   public:
    Display(Rectangle box, Font displayFont);

    // Draw the calculator display with all elements
    void draw(const CalculatorState& calc, const Theme& theme,
              const std::string& perfInfo);

    // Helper function to truncate string to fit width
    std::string truncateToFit(const std::string& text, float fontSize,
                              float maxWidth) const;
};