#include "../includes/parser.h"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <numbers>
#include <stack>
#include <stdexcept>

std::optional<double> MathParser::evaluate(std::string_view expression) {
    if (expression.empty()) {
        throw std::runtime_error("Empty expression");
    }

    try {
        auto tokens = tokenize(expression);
        if (tokens.empty()) {
            throw std::runtime_error("Invalid expression format");
        }

        auto rpn = toRPN(tokens);
        return computeRPN(rpn);
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("Calculation error: ") + e.what());
    }
}

std::vector<std::string> MathParser::tokenize(std::string_view expr) {
    std::vector<std::string> tokens;
    tokens.reserve(expr.size() / 2); // Reasonable estimate

    for (std::size_t i = 0; i < expr.length(); ++i) {
        char c = expr[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            continue;
        }

        if (c == '-' && (tokens.empty() || tokens.back() == "(" || isOperator(tokens.back()))) {
            // Unary minus
            std::string numStr = "-";
            ++i;
            while (i < expr.length() && (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.')) {
                numStr += expr[i];
                ++i;
            }
            --i;
            tokens.push_back(std::move(numStr));
        } else if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            std::string numStr;
            while (i < expr.length() && (std::isdigit(static_cast<unsigned char>(expr[i])) || expr[i] == '.')) {
                numStr += expr[i];
                ++i;
            }
            --i;
            tokens.push_back(std::move(numStr));
        } else if (std::isalpha(static_cast<unsigned char>(c))) {
            std::string funcStr;
            while (i < expr.length() && std::isalpha(static_cast<unsigned char>(expr[i]))) {
                funcStr += expr[i];
                ++i;
            }
            --i;
            tokens.push_back(std::move(funcStr));
        } else {
            tokens.emplace_back(1, c);
        }
    }
    return tokens;
}

std::vector<std::string> MathParser::toRPN(const std::vector<std::string> &tokens) {
    std::vector<std::string> output;
    output.reserve(tokens.size());
    std::stack<std::string> opStack;

    for (const auto &token : tokens) {
        if (isNumber(token)) {
            output.push_back(token);
        } else if (isFunction(token)) {
            opStack.push(token);
        } else if (isOperator(token)) {
            while (!opStack.empty() && isOperator(opStack.top()) && precedence(opStack.top()) >= precedence(token)) {
                output.push_back(std::move(opStack.top()));
                opStack.pop();
            }
            opStack.push(token);
        } else if (token == "(") {
            opStack.push(token);
        } else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push_back(std::move(opStack.top()));
                opStack.pop();
            }
            if (!opStack.empty())
                opStack.pop();
            if (!opStack.empty() && isFunction(opStack.top())) {
                output.push_back(std::move(opStack.top()));
                opStack.pop();
            }
        }
    }

    while (!opStack.empty()) {
        output.push_back(std::move(opStack.top()));
        opStack.pop();
    }

    return output;
}

double MathParser::computeRPN(const std::vector<std::string> &rpn) {
    std::stack<double> stack;

    for (const auto &token : rpn) {
        if (isNumber(token)) {
            stack.push(std::stod(token));
        } else if (isOperator(token)) {
            if (stack.size() < 2) {
                throw std::runtime_error("Invalid expression");
            }
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(applyOperator(a, b, token));
        } else if (isFunction(token)) {
            if (stack.empty()) {
                throw std::runtime_error("Invalid function call");
            }
            double a = stack.top();
            stack.pop();
            stack.push(applyFunction(a, token));
        }
    }

    if (stack.empty()) {
        throw std::runtime_error("Empty expression");
    }
    return stack.top();
}

bool MathParser::isNumber(std::string_view token) noexcept {
    if (token.empty())
        return false;

    char *endPtr = nullptr;
    std::string tokenStr(token); // Need null-terminated for strtod
    std::strtod(tokenStr.c_str(), &endPtr);
    return *endPtr == '\0';
}

constexpr bool MathParser::isOperator(std::string_view token) noexcept {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

constexpr bool MathParser::isFunction(std::string_view token) noexcept {
    return token == "sin" || token == "cos" || token == "tan" || token == "log" || token == "ln" || token == "exp" || token == "sqrt" ||
           token == "asin" || token == "acos" || token == "atan" || token == "hyp";
}

constexpr int MathParser::precedence(std::string_view op) noexcept {
    if (op == "+" || op == "-")
        return 1;
    if (op == "*" || op == "/")
        return 2;
    if (op == "^")
        return 3;
    return 0;
}

double MathParser::applyOperator(double a, double b, std::string_view op) {
    if (op == "+")
        return a + b;
    if (op == "-")
        return a - b;
    if (op == "*")
        return a * b;
    if (op == "/") {
        if (b == 0.0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }
    if (op == "^") {
        if (a == 0.0 && b < 0.0) {
            throw std::runtime_error("Cannot raise zero to a negative power");
        }
        if (a < 0.0 && std::floor(b) != b) {
            throw std::runtime_error("Cannot compute imaginary results");
        }
        return std::pow(a, b);
    }
    throw std::runtime_error("Unknown operator");
}

double MathParser::applyFunction(double a, std::string_view func) {
    constexpr double DEG_TO_RAD = std::numbers::pi / 180.0;
    constexpr double RAD_TO_DEG = 180.0 / std::numbers::pi;

    // Trigonometric functions (using degrees)
    if (func == "sin")
        return std::sin(a * DEG_TO_RAD);
    if (func == "cos")
        return std::cos(a * DEG_TO_RAD);
    if (func == "tan") {
        if (std::fmod(std::abs(a - 90.0), 180.0) < 1e-10) {
            throw std::runtime_error("Tangent undefined at " + std::to_string(static_cast<int>(a)) + " degrees");
        }
        return std::tan(a * DEG_TO_RAD);
    }

    // Logarithmic functions
    if (func == "log") {
        if (a <= 0.0)
            throw std::runtime_error("Cannot compute log of non-positive number");
        return std::log10(a);
    }
    if (func == "ln") {
        if (a <= 0.0)
            throw std::runtime_error("Cannot compute ln of non-positive number");
        return std::log(a);
    }

    // Other functions
    if (func == "exp")
        return std::exp(a);
    if (func == "sqrt") {
        if (a < 0.0)
            throw std::runtime_error("Cannot compute sqrt of negative number");
        return std::sqrt(a);
    }

    // Inverse trigonometric functions
    if (func == "asin") {
        if (a < -1.0 || a > 1.0)
            throw std::runtime_error("asin argument must be in [-1, 1]");
        return std::asin(a) * RAD_TO_DEG;
    }
    if (func == "acos") {
        if (a < -1.0 || a > 1.0)
            throw std::runtime_error("acos argument must be in [-1, 1]");
        return std::acos(a) * RAD_TO_DEG;
    }
    if (func == "atan")
        return std::atan(a) * RAD_TO_DEG;

    // Hyperbolic
    if (func == "hyp")
        return std::hypot(a, a);

    throw std::runtime_error("Unknown function");
}