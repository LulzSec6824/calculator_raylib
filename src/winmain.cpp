#if defined(_WIN32) && !defined(_DEBUG)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Forward declaration of the main function from main.cpp
extern int main();

// Windows GUI application entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Simply call the standard main function
    return main();
}

#endif // defined(_WIN32) && !defined(_DEBUG)