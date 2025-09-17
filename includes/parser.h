#pragma once
#include <cmath>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

class MathParser {
   public:
    std::unique_ptr<double> evaluate(const std::string& expression,
                                     bool isDeg = true);

   private:
    std::vector<std::string> tokenize(const std::string& expr);
    std::vector<std::string> toRPN(const std::vector<std::string>& tokens);
    double computeRPN(const std::vector<std::string>& rpn, bool isDeg);
    bool isNumber(const std::string& token);
    bool isOperator(const std::string& token);
    bool isFunction(const std::string& token);
    int precedence(const std::string& op);
    double applyOperator(double a, double b, const std::string& op);
    double applyFunction(double a, const std::string& func, bool isDeg);
};
