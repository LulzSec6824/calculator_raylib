#include "../includes/calculator.h"

#include <iomanip>
#include <sstream>

#include "../includes/parser.h"

// Constructor initializes calculator state
CalculatorState::CalculatorState()
    : display("0"),
      expression(""),
      operand1(0),
      operand2(0),
      op(0),
      enteringSecond(false),
      justEvaluated(false),
      isDarkMode(false) {}

// Handles all button press events and updates calculator state accordingly
void HandleButtonPress(CalculatorState& state, int clicked) {
    std::string append;
    bool isFunction = false;
    switch (clicked) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            append = std::string(1, static_cast<char>(clicked));
            break;
        case '.':
            append = ".";
            break;
        case '+':
            append = "+";
            break;
        case '-':
            append = "-";
            break;
        case '*':
            append = "*";
            break;
        case '/':
            append = "/";
            break;
        case '^':
            append = "^";
            break;
        case '(':
            append = "(";
            break;
        case ')':
            append = ")";
            break;
        case 100:
            state.isDarkMode = !state.isDarkMode;
            return;
        case 101:  // C
            state.expression = "";
            state.display    = "0";
            return;
        case 102:  // Backspace
            if (!state.expression.empty()) state.expression.pop_back();
            if (state.display != "0" && !state.display.empty())
                state.display.pop_back();
            if (state.display.empty()) state.display = "0";
            return;
        case 103:  // +/-
            if (state.display[0] == '-')
                state.display = state.display.substr(1);
            else
                state.display = "-" + state.display;
            return;
        case 110:
            append     = "sin(";
            isFunction = true;
            break;
        case 111:
            append     = "cos(";
            isFunction = true;
            break;
        case 112:
            append     = "tan(";
            isFunction = true;
            break;
        case 113:
            append     = "log(";
            isFunction = true;
            break;
        case 114:
            append     = "ln(";
            isFunction = true;
            break;
        case 115:
            append     = "exp(";
            isFunction = true;
            break;
        case 116:
            append     = "sqrt(";
            isFunction = true;
            break;
        case 117:
            append     = "hyp(";
            isFunction = true;
            break;
        case 118:
            append     = "asin(";
            isFunction = true;
            break;
        case 119:
            append     = "acos(";
            isFunction = true;
            break;
        case 120:
            append     = "atan(";
            isFunction = true;
            break;
        // Add more for other buttons if needed
        case '=':
            try {
                MathParser parser;
                double result = parser.evaluate(state.expression);
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(10) << result;
                state.display = oss.str();
                state.display.erase(state.display.find_last_not_of('0') + 1,
                                    std::string::npos);
                if (!state.display.empty() && state.display.back() == '.')
                    state.display.pop_back();
                state.expression    = state.display;
                state.justEvaluated = true;
            } catch (...) {
                state.display = "Error";
            }
            return;
    }
    if (!append.empty()) {
        state.expression += append;
        if (isFunction) {
            state.display = "";
        } else {
            state.display += append;
        }
        state.justEvaluated = false;
    }
}