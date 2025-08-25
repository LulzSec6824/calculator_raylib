#include <memory>
#include <stdexcept>

#include "../raylib/src/raylib.h"

int main() {
    // Initialize window (required for raylib functions)
    InitWindow(100, 100, "Resource Exporter");

    try {
        // Export font as code
        Font font = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
        // NOTE: We are saving the font data as a C header file
        // This will generate a font_ubuntu.h file with the font data embedded
        // as a byte array and a function to load it. We will replace the
        // C-style initialization with C++-style to avoid warnings.
        if (ExportFontAsCode(font, "includes/font_ubuntu.h") == false) {
            throw std::runtime_error("Failed to export font");
        }
        UnloadFont(font);

        // Export icon as code
        Image icon = LoadImage("resource/calc.png");
        // NOTE: We are saving the icon data as a C header file
        // This will generate a icon_calc.h file with the icon data embedded as
        // a byte array and a function to load it. We will replace the C-style
        // initialization with C++-style to avoid warnings.
        if (ExportImageAsCode(icon, "includes/icon_calc.h") == false) {
            throw std::runtime_error("Failed to export image");
        }
        UnloadImage(icon);

    } catch (const std::exception& e) {
        CloseWindow();
        return 1;
    }

    // Close window
    CloseWindow();

    return 0;
}