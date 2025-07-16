# Raylib Calculator

[![CMake on multiple platforms](https://github.com/LulzSec6824/calculator_raylib/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/LulzSec6824/calculator_raylib/actions/workflows/cmake-multi-platform.yml)
[![Latest Release](https://img.shields.io/github/v/release/LulzSec6824/raylib-calculator)](https://github.com/LulzSec6824/calculator_raylib/releases)
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)

A simple, modern, and cross-platform calculator built with C++ and the Raylib library. This project showcases a clean user interface, immediate feedback, and a responsive design that adapts to window resizing. It supports basic arithmetic operations, handles errors gracefully, and is structured for easy extension and maintenance.

A modern calculator application built with C++ and [raylib](https://www.raylib.com/), featuring a Windows-style UI with rounded buttons, dark gray background, antialiased fonts, mouse input, and a responsive dual-display system for expressions and results.

## Features
- **Dual Display System:** Unified display box showing the full input expression at the top and the current result below, with white text on a seamless background matching the app's dark gray theme.
- **Interactive UI:** Mouse-clickable buttons with rounded corners (roundness factor of 0.4f for smooth aesthetics), hover effects, and precise collision detection.
- **Operations Supported:** Basic arithmetic (addition, subtraction, multiplication, division), decimal points, sign toggling (+/-), clear (C), backspace (←), and equals (=) with chained operation support (equals not shown in display for cleaner UX).
- **Error Handling:** Graceful management of division by zero, input overflow (truncates long expressions), and invalid operations.
- **Font Rendering:** Antialiased text using Ubuntu-Regular.ttf for a crisp, modern look across platforms.
- **Modular Architecture:** Code separated into logical components for easy maintenance and extension.
- **Responsive Layout:** Dynamic positioning with configurable padding, offsets, and scaling to fit various screen sizes.

## Architecture Overview
The project is structured for modularity:
- **main.cpp:** Handles window initialization, main loop, input processing, and rendering. Manages layout variables like screen dimensions, padding, and offsets.
- **button.h / button.cpp:** Defines button structures, creates the grid layout (4x5 buttons), handles drawing with rounded rectangles, and manages press/hover states.
- **calculator.h / calculator.cpp:** Manages calculator state (expression string, result), handles button press logic, performs evaluations using std::strtod, and updates display strings.
- **resource/**: Contains fonts and other assets.

This separation allows for easy addition of features like scientific functions or themes.

## Installation and Build Instructions
1. **Prerequisites:**
   - C++ compiler (e.g clang or MSVC)
   - [raylib](https://www.raylib.com/) library installed and linked
   - CMake 3.10+
2. **Clone the Repository:**
   ```sh
   git clone https://github.com/LulzSec6824/calculator_raylib.git
   cd calculator_raylib
   ```
3. **Build:**
   ```sh
   cmake -S . -B build
   cmake --build build
   ```
4. **Run:** Execute the binary in `build/` (e.g., `./ray` on Linux or `ray.exe` on Windows).

## Usage
- Launch the application to see the calculator window (400x500 pixels).
- Click buttons to input numbers and operators.
- The expression builds in the display; press '=' to evaluate (result becomes the start of the next expression).
- Use 'C' to clear, '←' to backspace, '+/-' to toggle sign, '.' for decimals.
- Close the window to exit.

## Project Structure
```
Calculator_Raylib
├── .clang-format
├── .gitignore
├── CMakeLists.txt
├── LICENSE
├── README.md
├── includes/
│   ├── button.h
│   └── calculator.h
├── resource/
│   └── Ubuntu-Regular.ttf
└── src/
    ├── button.cpp
    ├── calculator.cpp
    └── main.cpp
```

## Customization
- **Theme:** Modify colors in main.cpp or button.cpp for button styles.
- **Fonts:** Swap Ubuntu-Regular.ttf in resource/.
- **Extensions:** Add buttons in button.cpp and logic in calculator.cpp for features like sqrt or memory.

## Contributing
1. Fork the repository.
2. Create a feature branch (`git checkout -b `).
3. Commit changes (`git commit -m 'Added some Feature'`).
4. Push to the branch (`git push origin feature`).
5. Open a Pull Request.

Please follow the .clang-format for code style.

## License
GNU General Public License v2 - see [LICENSE](LICENSE).