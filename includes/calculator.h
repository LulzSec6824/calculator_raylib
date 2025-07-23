#pragma once
#include <string>
#include <vector>

struct CalculatorState {
    std::string display;
    std::string expression;
    std::vector<std::string> history;
    double operand1;
    double operand2;
    char op;
    bool enteringSecond;
    bool justEvaluated;
    bool isDarkMode;
    CalculatorState();
};

void HandleButtonPress(CalculatorState& state, int buttonId);