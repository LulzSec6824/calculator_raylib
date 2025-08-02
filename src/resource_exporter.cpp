#include "../raylib_v5/src/raylib.h"

int main(int argc, char* argv[]) {
    // Initialize window (required for raylib functions)
    InitWindow(100, 100, "Resource Exporter");
    
    // Export font as code
    Font font = LoadFontEx("resource/Ubuntu-Regular.ttf", 64, 0, 0);
    ExportFontAsCode(font, "includes/font_ubuntu.h");
    UnloadFont(font);
    
    // Export icon as code
    Image icon = LoadImage("resource/calc.png");
    ExportImageAsCode(icon, "includes/icon_calc.h");
    UnloadImage(icon);
    
    // Close window
    CloseWindow();
    
    return 0;
}