#include "../includes/calculator.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

#include "../includes/parser.h"

// Constructor initializes calculator state
CalculatorState::CalculatorState()
    : display("0"),
      expression(""),
      operand1(0),
      operand2(0),
      lastResult(0),
      op(0),
      enteringSecond(false),
      justEvaluated(false),
      isDarkMode(false),
      errorState(false),
      errorMessage("") {}

// Format a number for display, removing trailing zeros and decimal point if
// needed
std::string FormatNumber(double value, int precision = 10) {
    // Handle special cases
    if (std::isnan(value)) return "Error: NaN";
    if (std::isinf(value)) return value > 0 ? "Infinity" : "-Infinity";

    // Format with fixed precision
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    std::string result = oss.str();

    // Remove trailing zeros
    result.erase(result.find_last_not_of('0') + 1, std::string::npos);

    // Remove decimal point if it's the last character
    if (!result.empty() && result.back() == '.') result.pop_back();

    return result;
}

// Handles all button press events and updates calculator state accordingly
void HandleButtonPress(CalculatorState& state, int clicked) {
    // Clear error state when any button is pressed
    if (state.errorState) {
        state.errorState   = false;
        state.errorMessage = "";
        if (clicked != 101) {  // If not the Clear button
            state.display    = "0";
            state.expression = "";
        }
    }

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
        case 101:  // C - Clear
            state.expression   = "";
            state.display      = "0";
            state.errorState   = false;
            state.errorMessage = "";
            return;
        case 102:  // Backspace
            if (!state.expression.empty()) {
                // Handle function names (remove the whole function name)
                static const std::vector<std::string> functions = {
                    "sin(",  "cos(", "tan(",  "log(",  "ln(",  "exp(",
                    "sqrt(", "hyp(", "asin(", "acos(", "atan("};

                bool functionRemoved = false;
                for (const auto& func : functions) {
                    if (state.expression.size() >= func.size() &&
                        state.expression.substr(state.expression.size() -
                                                func.size()) == func) {
                        state.expression.erase(state.expression.size() -
                                               func.size());
                        functionRemoved = true;
                        break;
                    }
                }

                if (!functionRemoved) {
                    state.expression.pop_back();
                }
            }

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
        case 205:  // ANS button
        {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(10) << state.lastResult;
            std::string ansStr = oss.str();
            ansStr.erase(ansStr.find_last_not_of('0') + 1, std::string::npos);
            if (!ansStr.empty() && ansStr.back() == '.') ansStr.pop_back();

            state.expression += ansStr;
            if (state.display == "0") {
                state.display = ansStr;
            } else {
                state.display += ansStr;
            }
            state.justEvaluated = false;
        }
            return;
        // Add more for other buttons if needed
        case '=':
            try {
                if (state.expression.empty()) {
                    state.display = "0";
                    return;
                }

                // Check for unbalanced parentheses
                int openParens = 0;
                for (char c : state.expression) {
                    if (c == '(')
                        openParens++;
                    else if (c == ')')
                        openParens--;
                }

                // Auto-complete missing closing parentheses
                std::string evaluationExpr = state.expression;
                while (openParens > 0) {
                    evaluationExpr += ")";
                    openParens--;
                }

                MathParser parser;
                double result = parser.evaluate(evaluationExpr);

                // Format the result with proper precision
                std::string resultStr = FormatNumber(result);

                // Limit history size to prevent memory growth
                if (state.history.size() >= 5) {
                    state.history.erase(state.history.begin());
                }

                // Add to history with the actual expression used (with
                // auto-completed parentheses if any)
                if (evaluationExpr != state.expression) {
                    state.history.push_back(state.expression + ")..." + " = " +
                                            resultStr);
                } else {
                    state.history.push_back(state.expression + " = " +
                                            resultStr);
                }

                state.display       = resultStr;
                state.expression    = resultStr;
                state.lastResult    = result;
                state.justEvaluated = true;
            } catch (const std::exception& e) {
                // Provide more informative error message
                state.display      = "Error";
                state.expression   = "";
                state.errorState   = true;
                state.errorMessage = e.what();

                // Add error to history
                if (state.history.size() >= 5) {
                    state.history.erase(state.history.begin());
                }
                state.history.push_back("Error: " + std::string(e.what()));
            } catch (...) {
                state.display      = "Error";
                state.expression   = "";
                state.errorState   = true;
                state.errorMessage = "Unknown error";
            }
            return;
    }
    if (!append.empty()) {
        state.expression += append;
        if (isFunction) {
            state.display = append;
        } else {
            if (state.display == "0") {
                state.display = append;
            } else {
                state.display += append;
            }
        }
        state.justEvaluated = false;
    }
}