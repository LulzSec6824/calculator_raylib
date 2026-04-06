#pragma once

#include <array>
#include <string>
#include <string_view>
#include <vector>

// Calculator class encapsulating all state and logic
class Calculator {
  public:
    // Constants
    static constexpr std::size_t MAX_HISTORY_SIZE = 5;

    // Default constructor
    Calculator() = default;

    // Disable copy (calculator state should be unique)
    Calculator(const Calculator &)            = delete;
    Calculator &operator=(const Calculator &) = delete;

    // Enable move
    Calculator(Calculator &&) noexcept            = default;
    Calculator &operator=(Calculator &&) noexcept = default;

    // Handle button press events
    void handleButtonPress(int buttonId);

    // State accessors
    [[nodiscard]] std::string_view getDisplay() const noexcept { return display_; }
    [[nodiscard]] std::string_view getExpression() const noexcept { return expression_; }
    [[nodiscard]] const std::vector<std::string> &getHistory() const noexcept { return history_; }
    [[nodiscard]] double getLastResult() const noexcept { return lastResult_; }
    [[nodiscard]] bool isDarkMode() const noexcept { return isDarkMode_; }
    [[nodiscard]] bool hasError() const noexcept { return errorState_; }
    [[nodiscard]] std::string_view getErrorMessage() const noexcept { return errorMessage_; }

    // State modifiers
    void toggleDarkMode() noexcept { isDarkMode_ = !isDarkMode_; }
    void clear() noexcept;

  private:
    // Internal state
    std::string display_{"0"};
    std::string expression_;
    std::vector<std::string> history_;
    double lastResult_{0.0};
    bool isDarkMode_{false};
    bool errorState_{false};
    bool justEvaluated_{false};
    std::string errorMessage_;

    // Function names for parsing (constexpr array)
    static constexpr std::array<std::string_view, 11> FUNCTIONS = {"sin(",  "cos(", "tan(",  "log(",  "ln(",  "exp(",
                                                                   "sqrt(", "hyp(", "asin(", "acos(", "atan("};

    // Helper methods
    void handleDigit(char digit);
    void handleOperator(char op);
    void handleParenthesis(char paren);
    void handleFunction(int functionId);
    void handleEquals();
    void handleBackspace();
    void handleNegate();
    void handleAns();
    void clearError() noexcept;

    [[nodiscard]] static std::string formatNumber(double value, int precision = 10);
};

// Legacy compatibility alias (deprecated)
using CalculatorState = Calculator;

// Legacy function (deprecated - use Calculator::handleButtonPress)
inline void HandleButtonPress(Calculator &calc, int buttonId) { calc.handleButtonPress(buttonId); }