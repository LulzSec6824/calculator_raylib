#pragma once
#include <cmath>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

class MathParser {
   public:
    double evaluate(const std::string& expression);

   private:
    std::vector<std::string> tokenize(const std::string& expr);
    std::vector<std::string> toRPN(const std::vector<std::string>& tokens);
    double computeRPN(const std::vector<std::string>& rpn);
    bool isNumber(const std::string& token);
    bool isOperator(const std::string& token);
    bool isFunction(const std::string& token);
    int precedence(const std::string& op);
    double applyOperator(double a, double b, const std::string& op);
    double applyFunction(double a, const std::string& func);
};

double MathParser::evaluate(const std::string& expression) {
    auto tokens = tokenize(expression);
    auto rpn    = toRPN(tokens);
    return computeRPN(rpn);
}

std::vector<std::string> MathParser::tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : expr) {
        if (std::isspace(c)) continue;
        if (std::isdigit(c) || c == '.') {
            token += c;
        } else {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            if (std::string("+-*/^()").find(c) != std::string::npos) {
                tokens.push_back(std::string(1, c));
            } else {
                token += c;
            }
        }
    }
    if (!token.empty()) tokens.push_back(token);
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
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(applyOperator(a, b, token));
        } else if (isFunction(token)) {
            double a = stack.top();
            stack.pop();
            stack.push(applyFunction(a, token));
        }
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
    if (func == "sin") return std::sin(a);
    if (func == "cos") return std::cos(a);
    if (func == "tan") return std::tan(a);
    if (func == "log") return std::log10(a);
    if (func == "ln") return std::log(a);
    if (func == "exp") return std::exp(a);
    if (func == "sqrt") return std::sqrt(a);
    if (func == "asin") return std::asin(a);
    if (func == "acos") return std::acos(a);
    if (func == "atan") return std::atan(a);
    if (func == "hyp") return std::hypot(a, a);  // Example, adjust as needed
    throw std::runtime_error("Unknown function");
}