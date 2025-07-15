#pragma once
#include <string>

struct CalculatorState {
    std::string display;
    std::string expression;
    double operand1;
    double operand2;
    char op;
    bool enteringSecond;
    bool justEvaluated;
    CalculatorState();
};

void HandleButtonPress(CalculatorState& state, int buttonId);