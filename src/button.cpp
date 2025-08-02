#include "../includes/button.h"

#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

#include "../raylib_v5/src/raylib.h"

// Button categories for visual styling
enum class ButtonCategory { NUMBER, OPERATOR, FUNCTION, CONTROL, SPECIAL };

// Button category mapping
static std::unordered_map<int, ButtonCategory> buttonCategories = {
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
    // Define button labels and their corresponding IDs in a grid layout
    std::vector<std::vector<std::pair<std::string, int>>> layout = {
        {{"sin", 110}, {"cos", 111}, {"tan", 112}, {"log", 113}, {"ln", 114}},
        {{"<-", 102}, {"sqrt", 116}, {"x^y", '^'}, {"(", '('}, {")", ')'}},
        {{"7", '7'}, {"8", '8'}, {"9", '9'}, {"/", '/'}, {"*", '*'}},
        {{"4", '4'}, {"5", '5'}, {"6", '6'}, {"-", '-'}, {"+", '+'}},
        {{"1", '1'}, {"2", '2'}, {"3", '3'}, {"0", '0'}, {".", '.'}},
        {{"±", 103}, {"ANS", 205}, {"=", '='}, {"T", 100}, {"C", 101}}};

    std::vector<Button> buttons;
    for (size_t row = 0; row < layout.size(); ++row) {
        for (size_t col = 0; col < layout[row].size(); ++col) {
            Button btn;
            // Set button position and size
            btn.rect  = {static_cast<float>(leftOffset + col * (btnW + margin)),
                         static_cast<float>(topOffset + row * (btnH + margin)),
                         static_cast<float>(btnW), static_cast<float>(btnH)};
            btn.label = layout[row][col].first;
            btn.id    = layout[row][col].second;
            btn.texture = nullptr;  // Initialize texture pointer to nullptr
            // Adjust font size based on label length
            btn.fontSize = btn.label.length() > 2
                               ? 16
                               : (btn.label.length() > 1 ? 18 : 22);
            btn.labelSize =
                MeasureTextEx(font, btn.label.c_str(),
                              static_cast<float>(btn.fontSize), 0.0f);
            buttons.push_back(btn);
        }
    }
    return buttons;
}

// Draws all calculator buttons and highlights the one under the mouse cursor
void DrawButtons(const std::vector<Button>& buttons, const Font& font,
                 Vector2 mouse, const bool isDarkMode) {
    // Theme colors for buttons
    const Color numberBgLight = {240, 240, 240, 255};
    const Color numberBgDark  = {60, 60, 60, 255};

    const Color operatorBgLight = {230, 230, 250, 255};  // Lavender
    const Color operatorBgDark  = {70, 70, 90, 255};

    const Color functionBgLight = {230, 250, 230, 255};  // Light green
    const Color functionBgDark  = {50, 80, 50, 255};

    const Color controlBgLight = {250, 230, 230, 255};  // Light red
    const Color controlBgDark  = {80, 50, 50, 255};

    const Color specialBgLight = {250, 250, 210, 255};  // Light yellow
    const Color specialBgDark  = {90, 90, 40, 255};

    const Color hoverLight = {173, 216, 230, 255};  // Light blue
    const Color hoverDark  = {100, 149, 237, 255};  // Cornflower blue

    const Color textLight = BLACK;
    const Color textDark  = WHITE;

    for (const auto& btn : buttons) {
        // Get button category
        ButtonCategory category = buttonCategories.count(btn.id)
                                      ? buttonCategories[btn.id]
                                      : ButtonCategory::NUMBER;

        // Determine button color based on category and theme
        Color btnColor;
        Color textColor;

        // Check if button is being hovered
        bool isHovered = CheckCollisionPointRec(mouse, btn.rect);

        if (isHovered) {
            btnColor = isDarkMode ? hoverDark : hoverLight;
        } else {
            // Select color based on button category and theme
            switch (category) {
                case ButtonCategory::NUMBER:
                    btnColor = isDarkMode ? numberBgDark : numberBgLight;
                    break;
                case ButtonCategory::OPERATOR:
                    btnColor = isDarkMode ? operatorBgDark : operatorBgLight;
                    break;
                case ButtonCategory::FUNCTION:
                    btnColor = isDarkMode ? functionBgDark : functionBgLight;
                    break;
                case ButtonCategory::CONTROL:
                    btnColor = isDarkMode ? controlBgDark : controlBgLight;
                    break;
                case ButtonCategory::SPECIAL:
                    btnColor = isDarkMode ? specialBgDark : specialBgLight;
                    break;
            }
        }

        textColor = isDarkMode ? textDark : textLight;

        // Draw button with rounded corners
        DrawRectangleRounded(btn.rect, 0.3f, 0, btnColor);
        DrawRectangleRoundedLines(btn.rect, 0.3f, 0,
                                  isHovered ? DARKGRAY : GRAY);

        // If button has a texture, draw it instead of text
        if (btn.texture != nullptr) {
            // Calculate scaling to fit the texture within the button while
            // maintaining aspect ratio
            float scale = fmin((btn.rect.width - 10) / btn.texture->width,
                               (btn.rect.height - 10) / btn.texture->height);

            // Calculate position to center the texture in the button
            float textureWidth  = btn.texture->width * scale;
            float textureHeight = btn.texture->height * scale;
            Vector2 texturePos  = {
                btn.rect.x + (btn.rect.width - textureWidth) / 2,
                btn.rect.y + (btn.rect.height - textureHeight) / 2};

            // Draw the texture with appropriate tint based on dark mode
            Color tintColor = isDarkMode ? DARKGRAY : WHITE;
            DrawTextureEx(*btn.texture, texturePos, 0.0f, scale, tintColor);
        } else {
            // Draw text label for buttons without texture, using pre-calculated
            // size
            DrawTextEx(font, btn.label.c_str(),
                       {btn.rect.x + (btn.rect.width - btn.labelSize.x) / 2,
                        btn.rect.y + (btn.rect.height - btn.labelSize.y) / 2},
                       static_cast<float>(btn.fontSize), 0, textColor);

            // Add a subtle 3D effect for buttons
            if (!isHovered) {
                DrawRectangleLinesEx({btn.rect.x + 1, btn.rect.y + 1,
                                      btn.rect.width - 2, btn.rect.height - 2},
                                     1, Fade(WHITE, 0.3f));
            }
        }
    }
}