#include <raylib.h>

#include <sstream>
#include <string>

enum CalculatorState { INPUT_A, INPUT_B, CHOOSE_OP, SHOW_RESULT };

int main() {
    int screenWidth  = 800;
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Calculator in RAYLIB");
    SetTargetFPS(60);

    // Load CascadiaCode font from resource folder
    Font cascFont = LoadFont("resource/CascadiaCode-Regular.ttf");

    CalculatorState state = INPUT_A;
    std::string inputA, inputB;
    int a = 0, b = 0, choice = 0;
    double result          = 0;
    std::string resultText = "";

    while (!WindowShouldClose()) {
        IsWindowFullscreen();
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        } else if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            return 0;
        }
        // Input handling
        if (state == INPUT_A) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= '0' && key <= '9') || key == '-')
                    inputA += (char)key;
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_ENTER) && !inputA.empty()) {
                a     = std::stoi(inputA);
                state = INPUT_B;
            }
        } else if (state == INPUT_B) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= '0' && key <= '9') || key == '-')
                    inputB += (char)key;
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_ENTER) && !inputB.empty()) {
                b     = std::stoi(inputB);
                state = CHOOSE_OP;
            }
        } else if (state == CHOOSE_OP) {
            if (IsKeyPressed(KEY_ONE)) choice = 1;
            if (IsKeyPressed(KEY_TWO)) choice = 2;
            if (IsKeyPressed(KEY_THREE)) choice = 3;
            if (IsKeyPressed(KEY_FOUR)) choice = 4;
            if (choice > 0) {
                std::ostringstream oss;
                switch (choice) {
                    case 1:
                        result = a + b;
                        oss << "Result: " << result;
                        break;
                    case 2:
                        result = a - b;
                        oss << "Result: " << result;
                        break;
                    case 3:
                        result = a * b;
                        oss << "Result: " << result;
                        break;
                    case 4:
                        if (b != 0) {
                            result = double(a) / b;
                            oss << "Result: " << result;
                        } else {
                            oss << "Error: Division by zero!";
                        }
                        break;
                }
                resultText = oss.str();
                state      = SHOW_RESULT;
            }
        } else if (state == SHOW_RESULT) {
            if (IsKeyPressed(KEY_R)) {
                inputA.clear();
                inputB.clear();
                a = b = choice = 0;
                result         = 0;
                resultText.clear();
                state = INPUT_A;
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Use cascFont for all DrawText calls
        DrawTextEx(cascFont, "Calculator", {20, 20}, 30, 0, DARKGRAY);

        if (state == INPUT_A) {
            DrawTextEx(cascFont, "Enter first number (press Enter):", {20, 80},
                       20, 0, BLACK);
            DrawTextEx(cascFont, inputA.c_str(), {20, 110}, 30, 0, BLUE);
        } else if (state == INPUT_B) {
            DrawTextEx(cascFont, "Enter second number (press Enter):", {20, 80},
                       20, 0, BLACK);
            DrawTextEx(cascFont, inputB.c_str(), {20, 110}, 30, 0, BLUE);
        } else if (state == CHOOSE_OP) {
            DrawTextEx(cascFont, "Choose operation (press key):", {20, 80}, 20,
                       0, BLACK);
            DrawTextEx(cascFont, "1. Addition", {20, 110}, 20, 0, DARKGRAY);
            DrawTextEx(cascFont, "2. Subtraction", {20, 140}, 20, 0, DARKGRAY);
            DrawTextEx(cascFont, "3. Multiplication", {20, 170}, 20, 0,
                       DARKGRAY);
            DrawTextEx(cascFont, "4. Division", {20, 200}, 20, 0, DARKGRAY);
        } else if (state == SHOW_RESULT) {
            DrawTextEx(cascFont, resultText.c_str(), {20, 80}, 30, 0, RED);
            DrawTextEx(cascFont, "Press R to restart.", {20, 120}, 20, 0, GRAY);
        }

        EndDrawing();
    }
    // Unload font before closing window
    UnloadFont(cascFont);
    CloseWindow();
    return 0;
}