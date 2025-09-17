#include "../includes/parser.h"

#include <cmath>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::unique_ptr<double> MathParser::evaluate(const std::string& expression,
                                             bool isDeg) {
    if (expression.empty()) {
        throw std::runtime_error("Empty expression");
    }
    try {
        std::vector<std::string> tokens = tokenize(expression);
        if (tokens.empty()) {
            throw std::runtime_error("Invalid expression format");
        }

        std::vector<std::string> rpn = toRPN(tokens);
        return std::unique_ptr<double>(new double(computeRPN(rpn, isDeg)));
    } catch (const std::exception& e) {
        // Re-throw with more context if needed
        throw std::runtime_error(std::string("Calculation error: ") + e.what());
    }
}

std::vector<std::string> MathParser::tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    for (size_t i = 0; i < expr.length(); ++i) {
        char c = expr[i];
        if (isspace(c)) continue;

        if (c == '-' && (tokens.empty() || tokens.back() == "(" ||
                         isOperator(tokens.back()))) {
            // Unary minus
            std::string num_str = "-";
            i++;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                num_str += expr[i];
                i++;
            }
            i--;  // Decrement because the outer loop increments
            tokens.push_back(num_str);
        } else if (isdigit(c) || c == '.') {
            std::string num_str;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                num_str += expr[i];
                i++;
            }
            i--;
            tokens.push_back(num_str);
        } else if (isalpha(c)) {
            std::string func_str;
            while (i < expr.length() && isalpha(expr[i])) {
                func_str += expr[i];
                i++;
            }
            i--;
            tokens.push_back(func_str);
        } else {
            tokens.push_back(std::string(1, c));
        }
    }
    return tokens;
}

std::vector<std::string> MathParser::toRPN(
    const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> opStack;

    for (const std::string& token : tokens) {
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

double MathParser::computeRPN(const std::vector<std::string>& rpn, bool isDeg) {
    std::stack<double> stack;

    for (const std::string& token : rpn) {
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
            stack.push(applyFunction(a, token, isDeg));
        }
    }

    if (stack.empty()) {
        throw std::runtime_error("Empty expression");
    }
    return stack.top();
}

bool MathParser::isNumber(const std::string& token) {
    if (token.empty()) {
        return false;
    }
    char* p;
    strtod(token.c_str(), &p);
    return *p == 0;
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
    if (op == "/") {
        if (b == 0) {
            throw std::runtime_error("Division by zero");
        }
        return a / b;
    }
    if (op == "^") {
        // Check for invalid power operations
        if (a == 0 && b < 0) {
            throw std::runtime_error("Cannot raise zero to a negative power");
        }
        if (a < 0 && std::floor(b) != b) {
            throw std::runtime_error(
                "Cannot compute imaginary results (negative base with "
                "non-integer exponent)");
        }
        return std::pow(a, b);
    }
    throw std::runtime_error("Unknown operator: " + op);
}

double MathParser::applyFunction(double a, const std::string& func,
                                 bool isDeg) {
    double angle = a;
    if (isDeg) {
        angle = a * M_PI / 180.0;
    }

    if (func == "sin") {
        return std::sin(angle);
    }
    if (func == "cos") {
        return std::cos(angle);
    }
    if (func == "tan") {
        if (isDeg && std::fmod(std::abs(a - 90.0), 180.0) < 1e-10) {
            throw std::runtime_error("Tangent is undefined at " +
                                     std::to_string(static_cast<int>(a)) +
                                     " degrees");
        }
        return std::tan(angle);
    }

    // Logarithmic functions
    if (func == "log") {
        if (a <= 0) {
            throw std::runtime_error(
                "Cannot compute logarithm of non-positive number");
        }
        return std::log10(a);
    }
    if (func == "ln") {
        if (a <= 0) {
            throw std::runtime_error(
                "Cannot compute natural logarithm of non-positive number");
        }
        return std::log(a);
    }

    if (func == "exp") {
        return std::exp(a);
    }
    if (func == "sqrt") {
        if (a < 0) {
            throw std::runtime_error(
                "Cannot compute square root of negative number");
        }
        return std::sqrt(a);
    }

    // Inverse trigonometric functions
    if (func == "asin") {
        if (a < -1 || a > 1) {
            throw std::runtime_error(
                "Inverse sine argument must be between -1 and 1");
        }
        double result = std::asin(a);
        return isDeg ? result * 180.0 / M_PI : result;
    }
    if (func == "acos") {
        if (a < -1 || a > 1) {
            throw std::runtime_error(
                "Inverse cosine argument must be between -1 and 1");
        }
        double result = std::acos(a);
        return isDeg ? result * 180.0 / M_PI : result;
    }
    if (func == "atan") {
        double result = std::atan(a);
        return isDeg ? result * 180.0 / M_PI : result;
    }

    // Hyperbolic functions
    if (func == "hyp") {
        return std::hypot(a, a);
    }

    throw std::runtime_error("Unknown function: " + func);
}