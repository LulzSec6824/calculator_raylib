#pragma once
#include <string>
#include <vector>

struct CalculatorState {
    std::string display{"0"};
    std::string expression;
    std::vector<std::string> history;
    double operand1{0};
    double operand2{0};
    double lastResult{0};
    char op{0};
    bool enteringSecond{false};
    bool justEvaluated{false};
    bool isDarkMode{false};
    bool errorState{false};
    std::string errorMessage;
    CalculatorState() = default;
};

void HandleButtonPress(CalculatorState& state, int buttonId);