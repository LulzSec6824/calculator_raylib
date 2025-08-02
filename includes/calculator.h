#pragma once
#include <string>
#include <vector>

struct CalculatorState {
    std::string display;               // Current display value
    std::string expression;            // Current mathematical expression
    std::vector<std::string> history;  // Calculation history
    double operand1;                   // First operand for binary operations
    double operand2;                   // Second operand for binary operations
    double lastResult;         // Last calculated result (for ANS button)
    char op;                   // Current operation
    bool enteringSecond;       // Flag for entering second operand
    bool justEvaluated;        // Flag indicating expression was just evaluated
    bool isDarkMode;           // UI theme toggle
    bool errorState;           // Flag indicating an error occurred
    std::string errorMessage;  // Detailed error message
    CalculatorState();
};

void HandleButtonPress(CalculatorState& state, int buttonId);