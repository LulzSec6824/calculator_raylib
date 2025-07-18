#include "../includes/calculator.h"

#include <iomanip>
#include <sstream>

// Constructor initializes calculator state
CalculatorState::CalculatorState()
    : display("0"),
      expression(""),
      operand1(0),
      operand2(0),
      op(0),
      enteringSecond(false),
      justEvaluated(false) {}

// Handles all button press events and updates calculator state accordingly
void HandleButtonPress(CalculatorState& state, int clicked) {
    // Handle numeric button press
    if (clicked >= '0' && clicked <= '9') {
        if (state.display == "0" || state.justEvaluated) state.display = "";
        if (state.justEvaluated) state.expression = "";
        state.display += static_cast<char>(clicked);
        state.expression += static_cast<char>(clicked);
        state.justEvaluated = false;
    } else if (clicked == '.') {
        // Handle decimal point
        if (state.display.find('.') == std::string::npos) {
            state.display += ".";
            state.expression += ".";
        }
    } else if (clicked == 100) {  // CE (Clear Entry)
        if (state.justEvaluated) {
            // If we just evaluated, CE acts as C
            state.expression    = "";
            state.operand1      = 0;
            state.op            = 0;
            state.justEvaluated = false;
        } else {
            // Otherwise, just clear the current entry from the expression
            if (state.expression.size() >= state.display.size()) {
                state.expression.erase(state.expression.size() -
                                       state.display.size());
            }
        }
        state.display = "0";
    } else if (clicked == 101) {  // C (Clear All)
        state.display       = "0";
        state.expression    = "";
        state.operand1      = 0;
        state.op            = 0;
        state.justEvaluated = false;
    } else if (clicked == 102) {  // Backspace
        if (state.display.size() > 1)
            state.display.pop_back();
        else
            state.display = "0";
        if (!state.expression.empty()) state.expression.pop_back();
    } else if (clicked == 103) {  // +/− (Toggle sign)
        if (state.display[0] == '-')
            state.display = state.display.substr(1);
        else if (state.display != "0")
            state.display = "-" + state.display;
        // Expression sign toggle is not appended
    } else if (clicked == '+' || clicked == '-' || clicked == '*' ||
               clicked == '/') {
        // Handle operator button press
        if (state.justEvaluated) {
            state.expression = state.display;
        }
        state.operand1      = std::stod(state.display);
        state.op            = static_cast<char>(clicked);
        state.justEvaluated = false;
        state.expression += static_cast<char>(clicked);
        state.display = "0";
    } else if (clicked == '=') {
        // Handle equals button press and perform calculation
        state.operand2 = std::stod(state.display);
        double result  = 0;
        bool error     = false;
        switch (state.op) {
            case '+':
                result = state.operand1 + state.operand2;
                break;
            case '-':
                result = state.operand1 - state.operand2;
                break;
            case '*':
                result = state.operand1 * state.operand2;
                break;
            case '/':
                if (state.operand2 == 0)
                    error = true;
                else
                    result = state.operand1 / state.operand2;
                break;
            default:
                result = state.operand2;
        }
        std::ostringstream oss;
        if (error)
            oss << "Error";
        else
            oss << std::fixed << std::setprecision(10) << result;
        state.display = oss.str();
        // Remove trailing zeros
        state.display.erase(state.display.find_last_not_of('0') + 1,
                            std::string::npos);
        if (!state.display.empty() && state.display.back() == '.')
            state.display.pop_back();
        state.justEvaluated = true;
        state.op            = 0;
    }
}