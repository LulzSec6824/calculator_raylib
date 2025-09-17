#pragma once
#include <string>
#include <vector>

struct CalculatorState {
    std::string display{"0"};
    std::string expression;
    std::vector<std::string> history;
    double operand1{0.0};
    double operand2{0.0};
    double lastResult{0.0};
    char op{0};
    bool enteringSecond{false};
    bool justEvaluated{false};
    bool isDarkMode{false};
    bool errorState{false};
    std::string errorMessage;
    bool is_shiftActive{false};
    bool matrixInputMode{false};
    bool isDeg{true};

    // Reserve space for history to avoid frequent reallocations
    CalculatorState() {
        history.reserve(4);        // Reserve space for 10 history entries
        expression.reserve(256);   // Reserve space for typical expressions
        errorMessage.reserve(64);  // Reserve space for error messages
    }
};

void HandleButtonPress(CalculatorState& state, int buttonId);