# Raylib Calculator - Cross-Platform

[![CMake on multiple platforms](https://github.com/LulzSec6824/calculator_raylib/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/LulzSec6824/calculator_raylib/actions/workflows/cmake-multi-platform.yml)
[![Latest Release](https://img.shields.io/github/v/release/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib/releases)
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub top language](https://img.shields.io/github/languages/top/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib)
[![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib)
[![GitHub last commit](https://img.shields.io/github/last-commit/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib)

A simple, modern, and cross-platform calculator built with C++ and the Raylib library. This project showcases a clean user interface, immediate feedback, and a responsive design that adapts to window resizing. It supports basic arithmetic operations, handles errors gracefully, and is structured for easy extension and maintenance.

A modern calculator application built with C++ and [raylib](https://www.raylib.com/), featuring a Windows-style UI with rounded buttons, dark gray background, antialiased fonts, mouse input, and a responsive dual-display system for expressions and results.

This calculator is fully optimized for multiple platforms (Windows, macOS, Linux) and compilers (MSVC, Clang, GCC) with platform-specific optimizations and Link Time Optimization (LTO) support.

## Features
- **Dual Display System:** Unified display box showing the full input expression at the top and the current result below, with white text on a seamless background matching the app's dark gray theme.
- **Interactive UI:** Mouse-clickable buttons with rounded corners (roundness factor of 0.4f for smooth aesthetics), hover effects, and precise collision detection.
- **Operations Supported:** Basic arithmetic (addition, subtraction, multiplication, division), decimal points, sign toggling (+/-), clear (C), backspace (←), and equals (=) with chained operation support (equals not shown in display for cleaner UX).
- **Error Handling:** Graceful management of division by zero, input overflow (truncates long expressions), and invalid operations.
- **Font Rendering:** Antialiased text using Ubuntu-Regular.ttf for a crisp, modern look across platforms.
- **Modular Architecture:** Code separated into logical components for easy maintenance and extension.
- **Responsive Layout:** Dynamic positioning with configurable padding, offsets, and scaling to fit various screen sizes.
- **Cross-Platform Support:** Fully optimized for Windows, macOS, and Linux with platform-specific settings.
- **Multi-Compiler Support:** Compatible with MSVC, Clang, and GCC with compiler-specific optimizations.
- **Link Time Optimization:** Enhanced performance with LTO for all supported compilers in release builds.
- **No Console Window:** Windows release builds run without a console window for a cleaner user experience.

## Architecture Overview
The project is structured for modularity:
- **main.cpp:** Handles window initialization, main loop, input processing, and rendering. Manages layout variables like screen dimensions, padding, and offsets.
- **button.h / button.cpp:** Defines button structures, creates the grid layout (4x5 buttons), handles drawing with rounded rectangles, and manages press/hover states.
- **calculator.h / calculator.cpp:** Manages calculator state (expression string, result), handles button press logic, performs evaluations using std::strtod, and updates display strings.
- **resource/**: Contains fonts and other assets.

This separation allows for easy addition of features like scientific functions or themes.

## Installation and Build Instructions
1. **Prerequisites:**
   - C++ compiler (e.g., Clang, GCC, or MSVC)
   - CMake 3.10+
   - Git

2. **Clone the Repository:**
   ```sh
   git clone https://github.com/LulzSec6824/calculator_raylib.git
   cd calculator_raylib
   ```

3. **Build Options:**

   **Using provided build scripts (recommended):**
   - **Linux:**
     ```sh
     bash build.sh
     ```
   - **macOS:**
     ```sh
     bash build_macos.sh
     ```
   - **Windows (Git Bash):**
     ```sh
     bash bashg.sh
     ```
   - **Windows (PowerShell):**
     ```powershell
     .\build.ps1
     ```
   - **Windows (Command Prompt):**
     ```cmd
     build.bat
     ```

   **Manual build:**
   ```sh
   # Debug build
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build --config Debug
   
   # Release build with optimizations
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release
   
   # Release build with debug info
   cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
   cmake --build build --config RelWithDebInfo
   
   # Minimal size release build
   cmake -S . -B build -DCMAKE_BUILD_TYPE=MinSizeRel
   cmake --build build --config MinSizeRel
   ```

4. **Run:** 
   - The build scripts will automatically run the application after building
   - To run manually, execute the binary in the appropriate directory:
     ```sh
     # Linux/macOS
     ./build/ray
     
     # Windows
     .\build\Release\ray.exe
     ```

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
├── .github/
│   └── workflows/
│       └── cmake-multi-platform.yml
├── .gitignore
├── CMakeLists.txt          # Enhanced with cross-platform support
├── CMakePresets.json
├── LICENSE
├── README.md
├── bashg.sh                # Windows Git Bash build script
├── build.bat               # Windows Command Prompt build script
├── build.ps1               # Windows PowerShell build script
├── build.sh                # Linux build script
├── build_macos.sh          # macOS build script
├── includes/
│   ├── button.h
│   ├── calculator.h
│   └── parser.h
├── resource/
│   └── Ubuntu-Regular.ttf
└── src/
    ├── button.cpp
    ├── calculator.cpp
    ├── main.cpp
    ├── parser.cpp
    └── winmain.cpp         # Windows entry point for GUI mode
```

## Optimization Features
The project includes comprehensive optimization settings for different build types, compilers, and platforms:

### Platform-Specific Optimizations
- **Windows (MSVC, Clang):**
  - Multi-processor compilation (`/MP`) for faster builds
  - Static runtime libraries (`/MT`) for standalone executables
  - GUI mode in Release builds (no console window)
  - MSVC-specific preprocessor definitions for compatibility

- **macOS (Clang):**
  - Minimum macOS version setting for compatibility
  - Apple-specific compiler flags
  - Automatic Reference Counting (ARC) for Objective-C/C++ code

- **Linux (GCC, Clang):**
  - Position Independent Code (`-fPIC`) for shared libraries
  - Platform-specific linking options

### Compiler-Specific Optimizations

#### Release Build Optimizations
- **Link Time Optimization (LTO)** for all supported compilers:
  - MSVC: Whole program optimization (`/GL`) with link-time code generation (`/LTCG`)
  - GCC: Full LTO (`-flto`)
  - Clang: Thin LTO (`-flto=thin`) on Windows/Linux, Full LTO on macOS
- **Architecture-specific optimizations** (`-march=native` for GCC/Clang)
- **Fast math** for floating-point operations
- **Function inlining** and other compiler-specific optimizations
- **Windows GUI mode** in Release and MinSizeRel builds (no console window)

### Debug Build Features
- **Address Sanitizer** for GCC and Clang builds
- **Full debugging symbols**
- **Runtime checks** for MSVC builds

### Other Build Profiles
- **RelWithDebInfo**: Optimized build with debugging information
- **MinSizeRel**: Size-optimized build with LTO

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