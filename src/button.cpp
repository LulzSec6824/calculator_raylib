#include "../includes/button.h"

#include <algorithm>
#include <array>
#include <string_view>
#include <utility>

#include "../raylib/src/raylib.h"

// Compile-time category lookup using constexpr
namespace {

// Category mapping entry
struct CategoryEntry {
    int id;
    ButtonCategory category;
};

// Sorted array for binary search (sorted by id)
constexpr std::array<CategoryEntry, 30> CATEGORY_MAP = {{
    // Numbers (ASCII values)
    {'.', ButtonCategory::NUMBER},
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

    // Operators
    {'(', ButtonCategory::OPERATOR},
    {')', ButtonCategory::OPERATOR},
    {'*', ButtonCategory::OPERATOR},
    {'+', ButtonCategory::OPERATOR},
    {'-', ButtonCategory::OPERATOR},
    {'/', ButtonCategory::OPERATOR},
    {'=', ButtonCategory::SPECIAL},
    {'^', ButtonCategory::OPERATOR},

    // Control buttons
    {100, ButtonCategory::CONTROL}, // Theme
    {101, ButtonCategory::CONTROL}, // Clear
    {102, ButtonCategory::CONTROL}, // Backspace
    {103, ButtonCategory::CONTROL}, // Negate

    // Functions
    {110, ButtonCategory::FUNCTION},
    {111, ButtonCategory::FUNCTION},
    {112, ButtonCategory::FUNCTION},
    {113, ButtonCategory::FUNCTION},
    {114, ButtonCategory::FUNCTION},
    {115, ButtonCategory::FUNCTION},
    {116, ButtonCategory::FUNCTION},
    // Note: 117-120 would extend the array
}};

constexpr ButtonCategory lookupCategory(int id) noexcept {
    // Linear search is fine for small arrays; could use binary search if larger
    for (const auto &entry : CATEGORY_MAP) {
        if (entry.id == id)
            return entry.category;
    }
    // Special cases not in map
    if (id >= 117 && id <= 120)
        return ButtonCategory::FUNCTION;
    if (id == 205)
        return ButtonCategory::SPECIAL;
    return ButtonCategory::NUMBER; // Default
}

} // anonymous namespace

ButtonCategory Button::getCategory() const noexcept { return lookupCategory(id); }

std::vector<Button> CreateButtons(int btnW, int btnH, int margin, int topOffset, int leftOffset, const Font &font) {
    using ButtonDef = std::pair<std::string_view, int>;
    using ButtonRow = std::array<ButtonDef, 5>;

    constexpr std::array<ButtonRow, 6> LAYOUT = {{{ButtonDef{"sin", 110}, {"cos", 111}, {"tan", 112}, {"log", 113}, {"ln", 114}},
                                                  {ButtonDef{"<-", 102}, {"sqrt", 116}, {"x^y", '^'}, {"(", '('}, {")", ')'}},
                                                  {ButtonDef{"7", '7'}, {"8", '8'}, {"9", '9'}, {"/", '/'}, {"*", '*'}},
                                                  {ButtonDef{"4", '4'}, {"5", '5'}, {"6", '6'}, {"-", '-'}, {"+", '+'}},
                                                  {ButtonDef{"1", '1'}, {"2", '2'}, {"3", '3'}, {"0", '0'}, {".", '.'}},
                                                  {ButtonDef{"+/-", 103}, {"ANS", 205}, {"=", '='}, {"Theme", 100}, {"C", 101}}}};

    std::vector<Button> buttons;
    buttons.reserve(30);

    for (std::size_t row = 0; row < LAYOUT.size(); ++row) {
        for (std::size_t col = 0; col < LAYOUT[row].size(); ++col) {
            Rectangle rect = {static_cast<float>(leftOffset + static_cast<int>(col) * (btnW + margin)),
                              static_cast<float>(topOffset + static_cast<int>(row) * (btnH + margin)), static_cast<float>(btnW),
                              static_cast<float>(btnH)};
            buttons.emplace_back(rect, LAYOUT[row][col].first, LAYOUT[row][col].second, font);
        }
    }
    return buttons;
}

void DrawButtons(const std::vector<Button> &buttons, const Font &font, Vector2 mouse, bool isDarkMode) {
    struct ThemeColors {
        Color numberBg;
        Color operatorBg;
        Color functionBg;
        Color controlBg;
        Color specialBg;
        Color hover;
        Color text;
    };

    static constexpr ThemeColors LIGHT_THEME = {
        {240, 240, 240, 255}, // numberBg
        {230, 230, 250, 255}, // operatorBg
        {230, 250, 230, 255}, // functionBg
        {250, 230, 230, 255}, // controlBg
        {250, 250, 210, 255}, // specialBg
        {173, 216, 230, 255}, // hover
        {0, 0, 0, 255}        // text (BLACK)
    };

    static constexpr ThemeColors DARK_THEME = {
        {60, 60, 60, 255},    // numberBg
        {70, 70, 90, 255},    // operatorBg
        {50, 80, 50, 255},    // functionBg
        {80, 50, 50, 255},    // controlBg
        {90, 90, 40, 255},    // specialBg
        {100, 149, 237, 255}, // hover
        {255, 255, 255, 255}  // text (WHITE)
    };

    const ThemeColors &theme = isDarkMode ? DARK_THEME : LIGHT_THEME;

    for (const Button &btn : buttons) {
        const bool isHovered          = CheckCollisionPointRec(mouse, btn.rect);
        const ButtonCategory category = btn.getCategory();

        Color btnColor;
        if (isHovered) {
            btnColor = theme.hover;
        } else {
            switch (category) {
            case ButtonCategory::NUMBER:
                btnColor = theme.numberBg;
                break;
            case ButtonCategory::OPERATOR:
                btnColor = theme.operatorBg;
                break;
            case ButtonCategory::FUNCTION:
                btnColor = theme.functionBg;
                break;
            case ButtonCategory::CONTROL:
                btnColor = theme.controlBg;
                break;
            case ButtonCategory::SPECIAL:
                btnColor = theme.specialBg;
                break;
            default:
                btnColor = theme.numberBg;
                break;
            }
        }

        // Draw button with rounded corners
        DrawRectangleRounded(btn.rect, 0.3f, 0, btnColor);
        DrawRectangleRoundedLines(btn.rect, 0.3f, 0, isHovered ? DARKGRAY : GRAY);

        if (btn.texture != nullptr) {
            float scale        = std::min((btn.rect.width - 10.0f) / static_cast<float>(btn.texture->width),
                                          (btn.rect.height - 10.0f) / static_cast<float>(btn.texture->height));
            Vector2 texturePos = {btn.rect.x + (btn.rect.width - static_cast<float>(btn.texture->width) * scale) * 0.5f,
                                  btn.rect.y + (btn.rect.height - static_cast<float>(btn.texture->height) * scale) * 0.5f};
            DrawTextureEx(*btn.texture, texturePos, 0.0f, scale, isDarkMode ? DARKGRAY : WHITE);
        } else {
            Vector2 textPos = {btn.rect.x + (btn.rect.width - btn.labelSize.x) * 0.5f, btn.rect.y + (btn.rect.height - btn.labelSize.y) * 0.5f};
            DrawTextEx(font, btn.label.c_str(), textPos, static_cast<float>(btn.fontSize), 0, theme.text);

            if (!isHovered) {
                Rectangle innerRect = {btn.rect.x + 1, btn.rect.y + 1, btn.rect.width - 2, btn.rect.height - 2};
                DrawRectangleLinesEx(innerRect, 1, Fade(WHITE, 0.3f));
            }
        }
    }
}