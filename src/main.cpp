#include <raylib.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

struct Button {
    Rectangle rect;
    std::string label;
    int id;
};

int main() {
    int screenWidth  = 350;
    int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "Calculator in RAYLIB");
    SetTargetFPS(60);
    Font font = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    std::string display    = "0";
    std::string expression = "";
    double operand1 = 0, operand2 = 0;
    char op             = 0;
    bool enteringSecond = false, justEvaluated = false;

    // Button layout (label, id)
    std::vector<std::vector<std::pair<std::string, int>>> layout = {
        {{"CE", 100}, {"C", 101}, {"<-", 102}, {"/", '/'}},
        {{"7", '7'}, {"8", '8'}, {"9", '9'}, {"*", '*'}},
        {{"4", '4'}, {"5", '5'}, {"6", '6'}, {"-", '-'}},
        {{"1", '1'}, {"2", '2'}, {"3", '3'}, {"+", '+'}},
        {{"+/-", 103}, {"0", '0'}, {".", '.'}, {"=", '='}}};

    std::vector<Button> buttons;
    int btnW = 75, btnH = 60, margin = 10, topOffset = 120, leftOffset = 10;
    for (int row = 0; row < layout.size(); ++row) {
        for (int col = 0; col < layout[row].size(); ++col) {
            Button btn;
            btn.rect  = {(float)(leftOffset + col * (btnW + margin)),
                         (float)(topOffset + row * (btnH + margin)), (float)btnW,
                         (float)btnH};
            btn.label = layout[row][col].first;
            btn.id    = layout[row][col].second;
            buttons.push_back(btn);
        }
    }

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();
        int clicked   = -1;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (auto& btn : buttons) {
                if (CheckCollisionPointRec(mouse, btn.rect)) {
                    clicked = btn.id;
                    break;
                }
            }
        }

        if (clicked != -1) {
            if (clicked >= '0' && clicked <= '9') {
                if (display == "0" || justEvaluated) display = "";
                if (justEvaluated) expression = "";
                display += (char)clicked;
                expression += (char)clicked;
                justEvaluated = false;
            } else if (clicked == '.') {
                if (display.find('.') == std::string::npos) {
                    display += ".";
                    expression += ".";
                }
            } else if (clicked == 100 || clicked == 101) {  // CE or C
                display       = "0";
                expression    = "";
                operand1      = 0;
                op            = 0;
                justEvaluated = false;
            } else if (clicked == 102) {  // Backspace
                if (display.size() > 1)
                    display.pop_back();
                else
                    display = "0";
                if (!expression.empty()) expression.pop_back();
            } else if (clicked == 103) {  // +/-
                if (display[0] == '-')
                    display = display.substr(1);
                else if (display != "0")
                    display = "-" + display;
                // Expression sign toggle is not appended
            } else if (clicked == '+' || clicked == '-' || clicked == '*' ||
                       clicked == '/') {
                operand1      = std::stod(display);
                op            = (char)clicked;
                justEvaluated = false;
                expression += (char)clicked;
                display = "0";
            } else if (clicked == '=') {
                operand2      = std::stod(display);
                double result = 0;
                bool error    = false;
                switch (op) {
                    case '+':
                        result = operand1 + operand2;
                        break;
                    case '-':
                        result = operand1 - operand2;
                        break;
                    case '*':
                        result = operand1 * operand2;
                        break;
                    case '/':
                        if (operand2 == 0)
                            error = true;
                        else
                            result = operand1 / operand2;
                        break;
                    default:
                        result = operand2;
                }
                std::ostringstream oss;
                if (error)
                    oss << "Error";
                else
                    oss << std::fixed << std::setprecision(10) << result;
                display = oss.str();
                // Remove trailing zeros
                display.erase(display.find_last_not_of('0') + 1,
                              std::string::npos);
                if (display.back() == '.') display.pop_back();
                expression += "=";
                justEvaluated = true;
                op            = 0;
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display area (upper: expression, lower: current input/result)
        DrawRectangle(leftOffset, 30, btnW * 4 + margin * 3, 70, LIGHTGRAY);
        DrawTextEx(font, expression.c_str(), {(float)(leftOffset + 10), 38}, 24,
                   0, DARKGRAY);
        DrawTextEx(font, display.c_str(), {(float)(leftOffset + 10), 65}, 40, 0,
                   BLACK);

        // Draw buttons
        for (auto& btn : buttons) {
            Color btnColor =
                CheckCollisionPointRec(mouse, btn.rect) ? SKYBLUE : WHITE;
            DrawRectangleRec(btn.rect, btnColor);
            DrawRectangleLinesEx(btn.rect, 2, GRAY);
            int fontSize = 28;
            Vector2 textSize =
                MeasureTextEx(font, btn.label.c_str(), fontSize, 0);
            DrawTextEx(font, btn.label.c_str(),
                       {btn.rect.x + (btn.rect.width - textSize.x) / 2,
                        btn.rect.y + (btn.rect.height - textSize.y) / 2},
                       fontSize, 0, BLACK);
        }
        EndDrawing();
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}