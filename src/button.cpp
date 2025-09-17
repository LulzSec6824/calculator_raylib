#include "../includes/button.h"

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "../raylib/src/raylib.h"

// Button categories for visual styling
enum class ButtonCategory { NUMBER, OPERATOR, FUNCTION, CONTROL, SPECIAL };

// Button category mapping
static const std::map<int, ButtonCategory> buttonCategories = {
    // Numbers
    {'0', ButtonCategory::NUMBER},
    {'1', ButtonCategory::NUMBER},
    {'2', ButtonCategory::NUMBER},
    {'3', ButtonCategory::NUMBER},
    {'4', ButtonCategory::NUMBER},
    {'5', ButtonCategory::NUMBER},
    {'6', ButtonCategory::NUMBER},
    {'7', ButtonCategory::NUMBER},
    {'8', ButtonCategory::NUMBER},
    {'9', ButtonCategory::NUMBER},
    {'.', ButtonCategory::NUMBER},

    // Operators
    {'+', ButtonCategory::OPERATOR},
    {'-', ButtonCategory::OPERATOR},
    {'*', ButtonCategory::OPERATOR},
    {'/', ButtonCategory::OPERATOR},
    {'^', ButtonCategory::OPERATOR},
    {'(', ButtonCategory::OPERATOR},
    {')', ButtonCategory::OPERATOR},

    // Functions
    {110, ButtonCategory::FUNCTION},
    {111, ButtonCategory::FUNCTION},
    {112, ButtonCategory::FUNCTION},
    {113, ButtonCategory::FUNCTION},
    {114, ButtonCategory::FUNCTION},
    {115, ButtonCategory::FUNCTION},
    {116, ButtonCategory::FUNCTION},
    {117, ButtonCategory::FUNCTION},
    {118, ButtonCategory::FUNCTION},
    {119, ButtonCategory::FUNCTION},
    {120, ButtonCategory::FUNCTION},
    {121, ButtonCategory::FUNCTION},
    {122, ButtonCategory::FUNCTION},
    {123, ButtonCategory::FUNCTION},

    // Control buttons
    {100, ButtonCategory::CONTROL},
    {101, ButtonCategory::CONTROL},
    {102, ButtonCategory::CONTROL},
    {103, ButtonCategory::CONTROL},

    // Special buttons
    {205, ButtonCategory::SPECIAL},
    {'=', ButtonCategory::SPECIAL}};

// Creates and returns a vector of Button objects arranged in a calculator
// layout
std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset,
                                  int leftOffset, const Font& font) {
    using ButtonRow    = std::vector<std::pair<const char*, int>>;
    using ButtonLayout = std::array<ButtonRow, 7>;

    // Define button labels and their corresponding IDs in a grid layout
    // Using const char* to avoid string allocations
    static const ButtonLayout layout = {
        {{{"S", 121}, {"M", 122}, {"DEG", 123}, {"", 0}, {"", 0}},
         {{"sin", 110}, {"cos", 111}, {"tan", 112}, {"log", 113}, {"ln", 114}},
         {{"<->", 102}, {"sqrt", 116}, {"x^y", '^'}, {"(", '('}, {")", ')'}},
         {{"7", '7'}, {"8", '8'}, {"9", '9'}, {"/", '/'}, {"*", '*'}},
         {{"4", '4'}, {"5", '5'}, {"6", '6'}, {"-", '-'}, {"+", '+'}},
         {{"1", '1'}, {"2", '2'}, {"3", '3'}, {"0", '0'}, {".", '.'}},
         {{"+/-", 103}, {"ANS", 205}, {"=", '='}, {"Theme", 100}, {"C", 101}}}};

    std::vector<Button> buttons;
    buttons.reserve(35);  // Pre-allocate memory for efficiency

    for (size_t row = 0; row < layout.size(); ++row) {
        for (size_t col = 0; col < layout[row].size(); ++col) {
            if (layout[row][col].second == 0) continue;
            Rectangle rect = {
                static_cast<float>(leftOffset + col * (btnW + margin)),
                static_cast<float>(topOffset + row * (btnH + margin)),
                static_cast<float>(btnW), static_cast<float>(btnH)};
            buttons.emplace_back(rect, layout[row][col].first,
                                 layout[row][col].second, font);
        }
    }
    return buttons;
}

// Draws all calculator buttons and highlights the one under the mouse cursor
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse, const bool isDarkMode) {
    struct ThemeColors {
        Color numberBg;
        Color operatorBg;
        Color functionBg;
        Color controlBg;
        Color specialBg;
        Color hover;
        Color text;
    };

    const ThemeColors lightTheme = {
        {240, 240, 240, 255},  // numberBg
        {230, 230, 250, 255},  // operatorBg (Lavender)
        {230, 250, 230, 255},  // functionBg (Light green)
        {250, 230, 230, 255},  // controlBg (Light red)
        {250, 250, 210, 255},  // specialBg (Light yellow)
        {173, 216, 230, 255},  // hover (Light blue)
        BLACK                  // text
    };

    const ThemeColors darkTheme = {
        {60, 60, 60, 255},     // numberBg
        {70, 70, 90, 255},     // operatorBg
        {50, 80, 50, 255},     // functionBg
        {80, 50, 50, 255},     // controlBg
        {90, 90, 40, 255},     // specialBg
        {100, 149, 237, 255},  // hover (Cornflower blue)
        WHITE                  // text
    };

    const ThemeColors& theme = isDarkMode ? darkTheme : lightTheme;

    for (const Button& btn : buttons) {
        ButtonCategory category = buttonCategories.count(btn.id)
                                      ? buttonCategories.at(btn.id)
                                      : ButtonCategory::NUMBER;

        bool isHovered = CheckCollisionPointRec(mouse, btn.rect);
        Color btnColor = isHovered ? theme.hover : [&]() {
            switch (category) {
                case ButtonCategory::NUMBER:
                    return theme.numberBg;
                case ButtonCategory::OPERATOR:
                    return theme.operatorBg;
                case ButtonCategory::FUNCTION:
                    return theme.functionBg;
                case ButtonCategory::CONTROL:
                    return theme.controlBg;
                case ButtonCategory::SPECIAL:
                    return theme.specialBg;
                default:
                    return theme.numberBg;
            }
        }();

        // Draw button with rounded corners
        DrawRectangleRounded(btn.rect, 0.3f, 0, btnColor);
        DrawRectangleRoundedLines(btn.rect, 0.3f, 0,
                                  isHovered ? DARKGRAY : GRAY);

        if (btn.texture != nullptr) {
            float scale =
                std::min((btn.rect.width - 10.0f) / btn.texture->width,
                         (btn.rect.height - 10.0f) / btn.texture->height);

            Vector2 texturePos = {
                btn.rect.x +
                    (btn.rect.width - btn.texture->width * scale) * 0.5f,
                btn.rect.y +
                    (btn.rect.height - btn.texture->height * scale) * 0.5f};

            DrawTextureEx(*btn.texture, texturePos, 0.0f, scale,
                          isDarkMode ? DARKGRAY : WHITE);
        } else {
            Vector2 textPos = {
                btn.rect.x + (btn.rect.width - btn.labelSize.x) * 0.5f,
                btn.rect.y + (btn.rect.height - btn.labelSize.y) * 0.5f};

            DrawTextEx(font, btn.label.c_str(), textPos,
                       static_cast<float>(btn.fontSize), 0, theme.text);

            if (!isHovered) {
                Rectangle innerRect = {btn.rect.x + 1, btn.rect.y + 1,
                                       btn.rect.width - 2, btn.rect.height - 2};
                DrawRectangleLinesEx(innerRect, 1, Fade(WHITE, 0.3f));
            }
        }
    }
}