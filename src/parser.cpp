#include "../includes/parser.h"

#include <cmath>
#include <stdexcept>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double MathParser::evaluate(const std::string& expression) {
    auto tokens = tokenize(expression);
    auto rpn    = toRPN(tokens);
    return computeRPN(rpn);
}

std::vector<std::string> MathParser::tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string token;
    size_t i = 0;

    while (i < expr.length()) {
        if (std::isspace(expr[i])) {
            i++;
            continue;
        }

        // Check for function names
        if (std::isalpha(expr[i])) {
            std::string func;
            while (i < expr.length() && std::isalpha(expr[i])) {
                func += expr[i];
                i++;
            }
            tokens.push_back(func);
            continue;
        }

        // Check for numbers (including decimals)
        if (std::isdigit(expr[i]) || expr[i] == '.') {
            std::string num;
            while (i < expr.length() &&
                   (std::isdigit(expr[i]) || expr[i] == '.')) {
                num += expr[i];
                i++;
            }
            tokens.push_back(num);
            continue;
        }

        // Single character operators and parentheses
        if (std::string("+-*/^()").find(expr[i]) != std::string::npos) {
            tokens.push_back(std::string(1, expr[i]));
            i++;
            continue;
        }

        i++;
    }

    return tokens;
}

std::vector<std::string> MathParser::toRPN(
    const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> opStack;
    for (const auto& token : tokens) {
        if (isNumber(token)) {
            output.push_back(token);
        } else if (isFunction(token)) {
            opStack.push(token);
        } else if (isOperator(token)) {
            while (!opStack.empty() && isOperator(opStack.top()) &&
                   precedence(opStack.top()) >= precedence(token)) {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        } else if (token == "(") {
            opStack.push(token);
        } else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            opStack.pop();
            if (!opStack.empty() && isFunction(opStack.top())) {
                output.push_back(opStack.top());
                opStack.pop();
            }
        }
    }
    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }
    return output;
}

double MathParser::computeRPN(const std::vector<std::string>& rpn) {
    std::stack<double> stack;
    for (const auto& token : rpn) {
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

bool MathParser::isNumber(const std::string& token) {
    return !token.empty() && (std::isdigit(token[0]) || token[0] == '.');
}

bool MathParser::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" ||
           token == "^";
}

bool MathParser::isFunction(const std::string& token) {
    return token == "sin" || token == "cos" || token == "tan" ||
           token == "log" || token == "ln" || token == "exp" ||
           token == "sqrt" || token == "asin" || token == "acos" ||
           token == "atan" || token == "hyp";
}

int MathParser::precedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}

double MathParser::applyOperator(double a, double b, const std::string& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return a / b;
    if (op == "^") return std::pow(a, b);
    throw std::runtime_error("Unknown operator");
}

double MathParser::applyFunction(double a, const std::string& func) {
    if (func == "sin")
        return std::sin(a * M_PI / 180.0);  // Convert degrees to radians
    if (func == "cos")
        return std::cos(a * M_PI / 180.0);  // Convert degrees to radians
    if (func == "tan")
        return std::tan(a * M_PI / 180.0);  // Convert degrees to radians
    if (func == "log") return std::log10(a);
    if (func == "ln") return std::log(a);
    if (func == "exp") return std::exp(a);
    if (func == "sqrt") return std::sqrt(a);
    if (func == "asin")
        return std::asin(a) * 180.0 / M_PI;  // Convert radians to degrees
    if (func == "acos")
        return std::acos(a) * 180.0 / M_PI;  // Convert radians to degrees
    if (func == "atan")
        return std::atan(a) * 180.0 / M_PI;  // Convert radians to degrees
    if (func == "hyp") return std::hypot(a, a);
    throw std::runtime_error("Unknown function");
}