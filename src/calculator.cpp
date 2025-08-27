#include "../includes/calculator.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "../includes/parser.h"

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
    auto lastNonZero = result.find_last_not_of('0');
    if (lastNonZero != std::string::npos) {
        result.erase(lastNonZero + 1);
    }

    // Remove decimal point if it's the last character
    if (!result.empty() && result.back() == '.') {
        result.pop_back();
    }

    return result;
}

// Handles all button press events and updates calculator state accordingly
void HandleButtonPress(CalculatorState& state, int clicked) {
    static const std::array<std::string, 11> functions = {
        "sin(",  "cos(", "tan(",  "log(",  "ln(",  "exp(",
        "sqrt(", "hyp(", "asin(", "acos(", "atan("};
    // Clear error state when any button is pressed
    if (state.errorState) {
        state.errorState = false;
        state.errorMessage.clear();
        state.display = "0";
        state.expression.clear();
    }

    std::string append;
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
        case '9': {
            if (state.justEvaluated) {
                state.expression.clear();
                state.display       = "0";
                state.justEvaluated = false;
            }
            if (state.display == "0") {
                state.display = std::string(1, static_cast<char>(clicked));
            } else {
                state.display.push_back(static_cast<char>(clicked));
            }
            append = std::string(1, static_cast<char>(clicked));
            break;
        }
        case '.': {
            if (state.display.find('.') == std::string::npos) {
                state.display.push_back('.');
                append = ".";
            }
            break;
        }
        case '+':
        case '-':
        case '*':
        case '/':
        case '^': {
            if (state.justEvaluated) {
                state.expression    = FormatNumber(state.lastResult);
                state.justEvaluated = false;
            }
            append        = std::string(1, static_cast<char>(clicked));
            state.display = "0";
            break;
        }
        case '(':
        case ')': {
            append = std::string(1, static_cast<char>(clicked));
            break;
        }
        case 100: {  // Dark/Light Mode Toggle
            state.isDarkMode = !state.isDarkMode;
            return;
        }
        case 101: {  // C - Clear
            state.expression.clear();
            state.display    = "0";
            state.errorState = false;
            state.errorMessage.clear();
            return;
        }
        case 102: {  // Backspace
            if (state.justEvaluated) {
                state.expression.clear();
                state.display    = "0";
                state.errorState = false;
                state.errorMessage.clear();
                state.justEvaluated = false;
                return;
            }

            if (!state.expression.empty()) {
                // Enhanced backspace logic to remove trailing functions or
                // numbers
                bool found = false;
                // Try to match and remove a function name
                for (const auto& func : functions) {
                    if (state.expression.size() >= func.size() &&
                        state.expression.substr(state.expression.size() -
                                                func.size()) == func) {
                        state.expression.erase(state.expression.size() -
                                               func.size());
                        found = true;
                        break;
                    }
                }

                // If not a function, remove the last character
                if (!found) {
                    state.expression.pop_back();
                }
            }

            // Also update the display
            if (!state.display.empty() && state.display != "0") {
                state.display.pop_back();
                if (state.display.empty()) {
                    state.display = "0";
                }
            }

            return;
        }
        case 103: {  // +/-
            if (state.justEvaluated) {
                state.lastResult    = -state.lastResult;
                state.display       = FormatNumber(state.lastResult);
                state.expression    = state.display;
                state.justEvaluated = false;
                return;
            }

            if (state.expression.empty()) return;

            // Find where the last number starts
            size_t lastNumStart = 0;
            for (size_t i = state.expression.length() - 1; i != (size_t)-1;
                 --i) {
                if (strchr("()+-*/^", state.expression[i])) {
                    lastNumStart = i + 1;
                    break;
                }
            }

            std::string lastNumStr = state.expression.substr(lastNumStart);
            if (lastNumStr.empty()) return;

            // Case 1: Number is parenthesized, e.g., "(-6)". Toggle to "6".
            if (lastNumStr.length() > 2 && lastNumStr.front() == '(' &&
                lastNumStr.back() == ')') {
                std::string positiveNum =
                    lastNumStr.substr(2, lastNumStr.length() - 3);
                state.expression.replace(lastNumStart, lastNumStr.length(),
                                         positiveNum);
                state.display = positiveNum;
            } else {  // Case 2: Number is not parenthesized. Toggle its sign.
                double lastNum        = std::stod(lastNumStr);
                lastNum               = -lastNum;
                std::string newNumStr = FormatNumber(lastNum);

                // Wrap with parentheses if it's negative and follows another
                // number/operator
                if (lastNum < 0 && lastNumStart > 0) {
                    char prevChar = state.expression[lastNumStart - 1];
                    if (prevChar != '(') {
                        newNumStr = "(" + newNumStr + ")";
                    }
                }

                state.expression.replace(lastNumStart, lastNumStr.length(),
                                         newNumStr);
                state.display = FormatNumber(lastNum);
            }
            return;
        }
        case 110:
        case 111:
        case 112:
        case 113:
        case 114:
        case 115:
        case 116:
        case 117:
        case 118:
        case 119:
        case 120: {
            if (state.justEvaluated) {
                state.expression.clear();
                state.justEvaluated = false;
            }

            append = functions[static_cast<size_t>(clicked - 110)];
            break;
        }
        case 205: {  // ANS button
            append = FormatNumber(state.lastResult);
            break;
        }
        case '=': {
            try {
                if (state.expression.empty()) {
                    state.display = "0";
                    return;
                }

                auto evalExpr = state.expression;
                auto openParens =
                    std::count(evalExpr.begin(), evalExpr.end(), '(') -
                    std::count(evalExpr.begin(), evalExpr.end(), ')');
                evalExpr.append(openParens, ')');

                MathParser parser;
                auto result    = *parser.evaluate(evalExpr);
                auto resultStr = FormatNumber(result);

                if (state.history.size() >= 5) {
                    state.history.erase(state.history.begin());
                }
                state.history.push_back(state.expression + " = " + resultStr);

                state.display       = resultStr;
                state.expression    = resultStr;
                state.lastResult    = result;
                state.justEvaluated = true;
            } catch (const std::exception& e) {
                state.display = "Error";
                state.expression.clear();
                state.errorState   = true;
                state.errorMessage = e.what();
            }
            return;
        }
    }

    if (!append.empty()) {
        state.expression += append;
        state.justEvaluated = false;
    }
}