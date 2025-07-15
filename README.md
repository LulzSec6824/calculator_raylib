# Raylib Windows-like Calculator

A modern, visually appealing calculator application built with C++ and [raylib](https://www.raylib.com/), inspired by the Windows Calculator. It features a clean UI, smooth font rendering with Ubuntu font, and intuitive mouse-driven input.

## Features

- **Windows Calculator UI**: Familiar button grid and dual-display system (expression and result).
- **Antialiased Fonts**: Uses Ubuntu-Regular.ttf with bilinear filtering for crisp text.
- **Mouse Input**: Click buttons to enter numbers and operations.
- **Standard Operations**: Supports addition, subtraction, multiplication, division, decimal input, sign toggle, clear, and backspace.
- **Real-time Expression Display**: Shows the full input expression above the result, updating as you type.
- **Responsive Layout**: UI elements are aligned and centered for a polished look.

## Screenshots

![Calculator Screenshot](screenshot.png)

## Getting Started

### Prerequisites
- C++17 or later
- [raylib](https://www.raylib.com/) (v4.0+ recommended)
- CMake (for building)

### Build Instructions

1. **Clone the repository**
   ```sh
   git clone <your-repo-url>
   cd ray
   ```
2. **Build with CMake**
   ```sh
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3. **Run the calculator**
   ```sh
   ./ray
   ```

### Font
The Ubuntu-Regular.ttf font is included in the `resource/` directory and is automatically copied to the build directory.

## Project Structure
```
├── CMakeLists.txt
├── resource/
│   └── Ubuntu-Regular.ttf
└── src/
    └── main.cpp
```

## Customization
- To change the font, replace `resource/Ubuntu-Regular.ttf` and update the font loading path in `main.cpp`.
- UI colors and layout can be tweaked in `main.cpp`.

## License
This project is open source and available under the MIT License.

---

**Made with [raylib](https://www.raylib.com/) and Ubuntu font for a beautiful, modern calculator experience.**