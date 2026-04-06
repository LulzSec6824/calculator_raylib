#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

class MathParser {
  public:
    // Evaluate a mathematical expression and return the result
    [[nodiscard]] std::optional<double> evaluate(std::string_view expression);

  private:
    // Tokenization and parsing
    [[nodiscard]] std::vector<std::string> tokenize(std::string_view expr);
    [[nodiscard]] std::vector<std::string> toRPN(const std::vector<std::string> &tokens);
    [[nodiscard]] double computeRPN(const std::vector<std::string> &rpn);

    // Token classification (constexpr for compile-time evaluation where possible)
    [[nodiscard]] static bool isNumber(std::string_view token) noexcept;
    [[nodiscard]] static constexpr bool isOperator(std::string_view token) noexcept;
    [[nodiscard]] static constexpr bool isFunction(std::string_view token) noexcept;
    [[nodiscard]] static constexpr int precedence(std::string_view op) noexcept;

    // Computation
    [[nodiscard]] static double applyOperator(double a, double b, std::string_view op);
    [[nodiscard]] static double applyFunction(double a, std::string_view func);
};
