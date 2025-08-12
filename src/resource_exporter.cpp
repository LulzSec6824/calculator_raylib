#include "../raylib/src/raylib.h"

int main() {
    // Initialize window (required for raylib functions)
    InitWindow(100, 100, "Resource Exporter");

    // Export font as code
    Font font = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
    // NOTE: We are saving the font data as a C header file
    // This will generate a font_ubuntu.h file with the font data embedded as a
    // byte array and a function to load it. We will replace the C-style
    // initialization with C++-style to avoid warnings.
    ExportFontAsCode(font, "includes/font_ubuntu.h");
    UnloadFont(font);

    // Export icon as code
    Image icon = LoadImage("resource/calc.png");
    // NOTE: We are saving the icon data as a C header file
    // This will generate a icon_calc.h file with the icon data embedded as a
    // byte array and a function to load it. We will replace the C-style
    // initialization with C++-style to avoid warnings.
    ExportImageAsCode(icon, "includes/icon_calc.h");
    UnloadImage(icon);

    // Close window
    CloseWindow();

    return 0;
}