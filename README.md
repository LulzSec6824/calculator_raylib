# Raylib Calculator

A modern calculator app built with C++ and [raylib](https://www.raylib.com/), featuring a Windows-style UI, antialiased fonts, mouse input, and a responsive dual-display system.

## Features
- **Dual Display:** Shows the full input expression and the current result in a single, unified display box.
- **Modular Codebase:** Logic is separated into multiple files for clarity and maintainability:
  - `src/main.cpp`: Application entry point and UI loop
  - `src/button.h` / `src/button.cpp`: Button layout and rendering
  - `src/calculator.h` / `src/calculator.cpp`: Calculator state and logic
- **Antialiased Fonts:** Uses Ubuntu-Regular for crisp, modern text rendering.
- **Mouse Input:** Fully interactive button-based input.
- **Standard Operations:** Supports addition, subtraction, multiplication, division, decimal input, sign toggle, clear, backspace, and equals.
- **Overflow Handling:** Input and output are truncated to fit the display area.
- **Responsive Layout:** UI scales and aligns elements for a polished look.

## Build Instructions
1. **Dependencies:**
   - [raylib](https://www.raylib.com/) (install and link as per your OS)
   - CMake (version 3.10 or higher)
2. **Build Steps:**
   ```sh
   cmake -S . -B build
   cmake --build build
   ```
3. **Run:**
   - The executable will be in the `build/` directory.

## Project Structure
```
ray/
├── CMakeLists.txt
├── README.md
├── LICENSE
├── resource/
│   └── Ubuntu-Regular.ttf
└── src/
    ├── main.cpp
    ├── button.h
    ├── button.cpp
    ├── calculator.h
    └── calculator.cpp
```

## Customization
- **Fonts:** Replace `resource/Ubuntu-Regular.ttf` with your preferred font.
- **Button Layout:** Edit `button.cpp` for custom button arrangements.
- **Logic:** Extend `calculator.cpp` for advanced operations or scientific functions.

## License
This project is licensed under the GNU General Public License v2. See the [LICENSE](LICENSE) file for details.