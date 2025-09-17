#pragma once
#include <cstddef>

namespace CalcConstants {
constexpr int BUTTON_ROWS        = 7;
constexpr int BUTTON_HEIGHT      = 45;
constexpr int BUTTON_SPACING     = 15;
constexpr int BUTTON_WIDTH       = 65;
constexpr int DISPLAY_BOX_HEIGHT = 200;
constexpr int SIDE_PADDING       = BUTTON_SPACING / 2;

constexpr int CALCULATOR_WIDTH  = 5 * BUTTON_WIDTH + 4 * BUTTON_SPACING;
constexpr int CALCULATOR_HEIGHT = DISPLAY_BOX_HEIGHT + BUTTON_SPACING +
                                  BUTTON_ROWS * BUTTON_HEIGHT +
                                  (BUTTON_ROWS - 1) * BUTTON_SPACING;

constexpr int SCREEN_WIDTH  = CALCULATOR_WIDTH + 2 * SIDE_PADDING;
constexpr int SCREEN_HEIGHT = CALCULATOR_HEIGHT + 2 * BUTTON_SPACING;

constexpr float DISPLAY_FONT_SIZE = 54.0f;
constexpr float EXPR_FONT_SIZE    = 24.0f;
constexpr float HISTORY_FONT_SIZE = 20.0f;
constexpr float STATUS_FONT_SIZE  = 16.0f;

constexpr float HISTORY_START_Y     = 10.0f;
constexpr float HISTORY_LINE_HEIGHT = 25.0f;

constexpr double SMOOTHING_FACTOR = 0.1;
constexpr int TARGET_FPS          = 60;

constexpr size_t BUTTON_RESERVE_COUNT       = 35;
constexpr size_t HISTORY_RESERVE_COUNT      = 10;
constexpr size_t EXPRESSION_RESERVE_SIZE    = 256;
constexpr size_t ERROR_MESSAGE_RESERVE_SIZE = 64;
constexpr size_t NUMBER_RESERVE_SIZE        = 16;
constexpr size_t FUNCTION_RESERVE_SIZE      = 8;
constexpr size_t PERFORMANCE_BUFFER_SIZE    = 128;
}