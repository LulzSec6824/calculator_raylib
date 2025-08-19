#include "../includes/calculator.h"

#include <algorithm>
#include <cmath>
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
        state.display      = "0";
        state.expression   = "";
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
        case '9':
            if (state.justEvaluated) {
                state.expression    = "";
                state.display       = "0";
                state.justEvaluated = false;
            }
            if (state.display == "0") {
                state.display = std::string(1, static_cast<char>(clicked));
            } else {
                state.display += std::string(1, static_cast<char>(clicked));
            }
            append = std::string(1, static_cast<char>(clicked));
            break;
        case '.':
            if (state.display.find('.') == std::string::npos) {
                state.display += ".";
                append = ".";
            }
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            if (state.justEvaluated) {
                state.expression    = FormatNumber(state.lastResult);
                state.justEvaluated = false;
            }
            append        = std::string(1, static_cast<char>(clicked));
            state.display = "0";  // Reset display for the next number
            break;
        case '(':
        case ')':
            append = std::string(1, static_cast<char>(clicked));
            break;
        case 100:  // Dark/Light Mode Toggle
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
                state.expression.pop_back();
            }
            if (state.display.length() > 1) {
                state.display.pop_back();
            } else {
                state.display = "0";
            }
            return;
        case 103:  // +/-
            if (state.display != "0" && !state.display.empty()) {
                std::string old_display = state.display;
                bool was_negative       = (old_display[0] == '-');

                // Toggle sign on display
                if (was_negative) {
                    state.display.erase(0, 1);
                } else {
                    state.display.insert(0, 1, '-');
                }

                // Now update the expression
                if (state.justEvaluated) {
                    state.expression    = state.display;
                    state.justEvaluated = false;
                    return;
                }

                // Find what to replace in the expression
                std::string to_replace = old_display;
                if (was_negative) {
                    // It might have been wrapped in parens, e.g. `5+(-3)`
                    if (state.expression.size() >= old_display.size() + 2 &&
                        state.expression.substr(state.expression.size() -
                                                (old_display.size() + 2)) ==
                            "(" + old_display + ")") {
                        to_replace = "(" + old_display + ")";
                    }
                }

                if (state.expression.size() >= to_replace.size() &&
                    state.expression.substr(state.expression.size() -
                                            to_replace.size()) == to_replace) {
                    state.expression.resize(state.expression.size() -
                                            to_replace.size());

                    std::string to_append = state.display;
                    if (!was_negative) {  // it is now negative
                        if (!state.expression.empty()) {
                            char last_char = state.expression.back();
                            if (last_char == '+' || last_char == '-' ||
                                last_char == '*' || last_char == '/' ||
                                last_char == '^') {
                                to_append = "(" + state.display + ")";
                            }
                        }
                    }
                    state.expression += to_append;
                }
            }
            return;
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
        case 120:
            if (state.justEvaluated) {
                state.expression    = "";
                state.justEvaluated = false;
            }
            switch (clicked) {
                case 110:
                    append = "sin(";
                    break;
                case 111:
                    append = "cos(";
                    break;
                case 112:
                    append = "tan(";
                    break;
                case 113:
                    append = "log(";
                    break;
                case 114:
                    append = "ln(";
                    break;
                case 115:
                    append = "exp(";
                    break;
                case 116:
                    append = "sqrt(";
                    break;
                case 117:
                    append = "hyp(";
                    break;
                case 118:
                    append = "asin(";
                    break;
                case 119:
                    append = "acos(";
                    break;
                case 120:
                    append = "atan(";
                    break;
            }
            break;
        case 205:  // ANS button
            append = FormatNumber(state.lastResult);
            break;
        case '=':
            try {
                if (state.expression.empty()) {
                    state.display = "0";
                    return;
                }

                // Auto-complete missing closing parentheses
                std::string evalExpr = state.expression;
                int openParens =
                    std::count(evalExpr.begin(), evalExpr.end(), '(') -
                    std::count(evalExpr.begin(), evalExpr.end(), ')');
                while (openParens > 0) {
                    evalExpr += ")";
                    openParens--;
                }

                MathParser parser;
                double result = *parser.evaluate(evalExpr);

                std::string resultStr = FormatNumber(result);

                if (state.history.size() >= 5) {
                    state.history.erase(state.history.begin());
                }
                state.history.push_back(state.expression + " = " + resultStr);

                state.display    = resultStr;
                state.expression = resultStr;  // Keep expression as the result
                                               // for potential chaining
                state.lastResult    = result;
                state.justEvaluated = true;
            } catch (const std::exception& e) {
                state.display      = "Error";
                state.expression   = "";
                state.errorState   = true;
                state.errorMessage = e.what();
            }
            return;
    }

    if (!append.empty()) {
        state.expression += append;
        state.justEvaluated = false;
    }
}