#include "../includes/calculator.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "../includes/parser.h"

// Format a number for display, removing trailing zeros
std::string Calculator::formatNumber(double value, int precision) {
    if (std::isnan(value))
        return "Error: NaN";
    if (std::isinf(value))
        return value > 0 ? "Infinity" : "-Infinity";

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    std::string result = oss.str();

    // Remove trailing zeros
    if (auto pos = result.find_last_not_of('0'); pos != std::string::npos) {
        result.erase(pos + 1);
    }

    // Remove trailing decimal point
    if (!result.empty() && result.back() == '.') {
        result.pop_back();
    }

    return result;
}

void Calculator::clear() noexcept {
    expression_.clear();
    display_    = "0";
    errorState_ = false;
    errorMessage_.clear();
    justEvaluated_ = false;
}

void Calculator::clearError() noexcept {
    if (errorState_) {
        errorState_ = false;
        errorMessage_.clear();
        display_ = "0";
        expression_.clear();
    }
}

void Calculator::handleDigit(char digit) {
    if (justEvaluated_) {
        expression_.clear();
        display_       = "0";
        justEvaluated_ = false;
    }

    if (display_ == "0") {
        display_ = std::string(1, digit);
    } else {
        display_.push_back(digit);
    }
    expression_.push_back(digit);
}

void Calculator::handleOperator(char op) {
    if (justEvaluated_) {
        expression_    = formatNumber(lastResult_);
        justEvaluated_ = false;
    }
    expression_.push_back(op);
    display_ = "0";
}

void Calculator::handleParenthesis(char paren) { expression_.push_back(paren); }

void Calculator::handleFunction(int functionId) {
    if (justEvaluated_) {
        expression_.clear();
        justEvaluated_ = false;
    }

    const auto index = static_cast<std::size_t>(functionId - 110);
    if (index < FUNCTIONS.size()) {
        expression_ += FUNCTIONS[index];
    }
}

void Calculator::handleBackspace() {
    if (justEvaluated_) {
        clear();
        return;
    }

    if (!expression_.empty()) {
        bool found = false;
        // Try to match and remove a function name
        for (const auto &func : FUNCTIONS) {
            if (expression_.size() >= func.size() && expression_.compare(expression_.size() - func.size(), func.size(), func) == 0) {
                expression_.erase(expression_.size() - func.size());
                found = true;
                break;
            }
        }

        if (!found) {
            expression_.pop_back();
        }
    }

    if (!display_.empty() && display_ != "0") {
        display_.pop_back();
        if (display_.empty()) {
            display_ = "0";
        }
    }
}

void Calculator::handleNegate() {
    if (justEvaluated_) {
        lastResult_    = -lastResult_;
        display_       = formatNumber(lastResult_);
        expression_    = display_;
        justEvaluated_ = false;
        return;
    }

    if (expression_.empty())
        return;

    // Find where the last number starts
    std::size_t lastNumStart = 0;
    for (std::size_t i = expression_.length(); i > 0; --i) {
        if (std::strchr("()+-*/^", expression_[i - 1])) {
            lastNumStart = i;
            break;
        }
    }

    std::string lastNumStr = expression_.substr(lastNumStart);
    if (lastNumStr.empty())
        return;

    // Handle parenthesized negative numbers
    if (lastNumStr.length() > 2 && lastNumStr.front() == '(' && lastNumStr.back() == ')') {
        std::string positiveNum = lastNumStr.substr(2, lastNumStr.length() - 3);
        expression_.replace(lastNumStart, lastNumStr.length(), positiveNum);
        display_ = positiveNum;
    } else {
        double lastNum        = std::stod(lastNumStr);
        lastNum               = -lastNum;
        std::string newNumStr = formatNumber(lastNum);

        if (lastNum < 0 && lastNumStart > 0 && expression_[lastNumStart - 1] != '(') {
            newNumStr = "(" + newNumStr + ")";
        }

        expression_.replace(lastNumStart, lastNumStr.length(), newNumStr);
        display_ = formatNumber(lastNum);
    }
}

void Calculator::handleAns() { expression_ += formatNumber(lastResult_); }

void Calculator::handleEquals() {
    try {
        if (expression_.empty()) {
            display_ = "0";
            return;
        }

        auto evalExpr   = expression_;
        auto openParens = std::count(evalExpr.begin(), evalExpr.end(), '(') - std::count(evalExpr.begin(), evalExpr.end(), ')');
        evalExpr.append(static_cast<std::size_t>(openParens), ')');

        MathParser parser;
        auto result = parser.evaluate(evalExpr);

        if (!result) {
            throw std::runtime_error("Evaluation failed");
        }

        auto resultStr = formatNumber(*result);

        if (history_.size() >= MAX_HISTORY_SIZE) {
            history_.erase(history_.begin());
        }
        history_.push_back(expression_ + " = " + resultStr);

        display_       = resultStr;
        expression_    = resultStr;
        lastResult_    = *result;
        justEvaluated_ = true;
    } catch (const std::exception &e) {
        display_ = "Error";
        expression_.clear();
        errorState_   = true;
        errorMessage_ = e.what();
    }
}

void Calculator::handleButtonPress(int buttonId) {
    clearError();

    switch (buttonId) {
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
        handleDigit(static_cast<char>(buttonId));
        break;

    case '.':
        if (display_.find('.') == std::string::npos) {
            display_.push_back('.');
            expression_.push_back('.');
        }
        break;

    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
        handleOperator(static_cast<char>(buttonId));
        break;

    case '(':
    case ')':
        handleParenthesis(static_cast<char>(buttonId));
        break;

    case 100: // Theme toggle
        toggleDarkMode();
        break;

    case 101: // Clear
        clear();
        break;

    case 102: // Backspace
        handleBackspace();
        break;

    case 103: // Negate
        handleNegate();
        break;

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
        handleFunction(buttonId);
        break;

    case 205: // ANS
        handleAns();
        break;

    case '=':
        handleEquals();
        break;

    default:
        break;
    }

    if (buttonId != 100 && buttonId != 101 && buttonId != '=') {
        justEvaluated_ = false;
    }
}