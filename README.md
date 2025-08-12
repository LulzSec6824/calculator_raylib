# Raylib Calculator - Cross-Platform

[![CMake on multiple platforms](https://github.com/LulzSec6824/calculator_raylib/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/LulzSec6824/calculator_raylib/actions/workflows/cmake-multi-platform.yml)
[![Latest Release](https://img.shields.io/github/v/release/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib/releases)
[![License: GPL v2](https://img.shields.io/badge/License-GPL_v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)
[![GitHub top language](https://img.shields.io/github/languages/top/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib)
[![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib)
[![GitHub last commit](https://img.shields.io/github/last-commit/LulzSec6824/calculator_raylib)](https://github.com/LulzSec6824/calculator_raylib)

A modern, cross-platform scientific calculator built with C++ and the Raylib library. This project features a clean user interface with light/dark theme support, immediate feedback with detailed error messages, performance metrics display, and a responsive design that adapts to window resizing. It supports basic arithmetic operations, scientific functions, handles errors gracefully, and is structured for extensibility.

## 🚀 Features

- **Cross-Platform:** Works on Windows, macOS, and Linux
- **Multi-Compiler Support:** MSVC, Clang, and GCC with optimized settings
- **Link Time Optimization (LTO):** Enhanced performance in release builds
- **Responsive UI:** Adapts to window resizing with configurable layouts
- **Light/Dark Theme:** Toggle between light and dark mode with the theme button
- **Performance Metrics:** Real-time display of FPS and frame time
- **Scientific Functions:** Support for sin, cos, tan, log, sqrt and more
- **Detailed Error Handling:** Informative error messages for calculation errors
- **Expression History:** View previous calculations with results
- **No Console Window:** Clean Windows GUI experience in release builds (optional)
- **Modern C++:** C++11 standard with clean architecture
- **Easy to Build:** Multiple build scripts and manual build options

## 📋 System Requirements

### Prerequisites

- **CMake 3.10+** (build system)
- **C++ Compiler** (one of the following):
  - **Windows:** MSVC 2019+, Clang 12+, or MinGW-w64
  - **macOS:** Xcode Command Line Tools (Clang)
  - **Linux:** GCC 9+ or Clang 12+
- **Git** (for cloning)

### Platform-Specific Requirements

#### Windows

- **MSVC:** Visual Studio 2019 or later with C++ development tools
- **MinGW:** MinGW-w64 with GCC 9+ or Clang

#### macOS

- **Xcode Command Line Tools:** `xcode-select --install`
- **Homebrew:** Optional but recommended for additional tools

#### Linux

- **Build Tools:** `build-essential` (Ubuntu/Debian) or equivalent
- **Development Libraries:** Usually pre-installed with build tools

## 🛠️ Quick Start

### Method 1: Using Build Scripts (Recommended)

### Windows

**Command Prompt:**

```cmd
# Clone repository
git clone https://github.com/LulzSec6824/calculator_raylib.git
cd calculator_raylib

# Build and run
build.bat
```

**Git Bash:**

```bash
# Clone repository
git clone https://github.com/LulzSec6824/calculator_raylib.git
cd calculator_raylib

# Build and run
bash build.sh
```

### macOS

```bash
# Clone repository
git clone https://github.com/LulzSec6824/calculator_raylib.git
cd calculator_raylib

# Build and run
bash build_macos.sh
```

### Linux

```bash
# Clone repository
git clone https://github.com/LulzSec6824/calculator_raylib.git
cd calculator_raylib

# Build and run
bash build.sh
```

### Method 2: Manual Build

#### Step 1: Clone Repository

```bash
git clone https://github.com/LulzSec6824/calculator_raylib.git
cd calculator_raylib
```

#### Step 2: Configure Build

Choose your preferred build type:

**Debug Build (Development):**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

**Release Build (Production):**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

**Release with Debug Info:**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

**Minimal Size Release:**

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=MinSizeRel
```

#### Step 3: Build Project

```bash
cmake --build build --config Release
```

#### Step 4: Run Application

```bash
# Windows
.\build\Release\ray.exe

# macOS/Linux
./build/ray
```

## 🔧 Advanced Build Configuration

### CMake Options

#### Compiler Selection

```bash
# Explicit compiler selection
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
```

#### Generator Selection

```bash
# Visual Studio 2022 (Windows)
cmake -S . -B build -G "Visual Studio 17 2022"

# Ninja (Cross-platform)
cmake -S . -B build -G Ninja

# Xcode (macOS)
cmake -S . -B build -G Xcode
```

#### Custom Install Directory

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/custom/install/path
```

### Platform-Specific Build Examples

#### Windows with MSVC

```bash
# Configure with MSVC
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
.\build\Release\ray.exe
```

#### Windows with MinGW

```bash
# Configure with MinGW
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
.\build\ray.exe
```

#### macOS with Xcode

```bash
# Configure with Xcode generator
cmake -S . -B build -G Xcode -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
./build/Release/ray
```

#### Linux with GCC

```bash
# Configure with GCC
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
./build/ray
```

## 🐛 Troubleshooting

### Common Issues and Solutions

#### CMake Not Found

```bash
# Windows: Install via Visual Studio Installer or Chocolatey
choco install cmake

# macOS: Install via Homebrew
brew install cmake

# Linux: Install via package manager
sudo apt install cmake build-essential  # Ubuntu/Debian
sudo yum install cmake gcc-c++ make     # CentOS/RHEL
sudo pacman -S cmake base-devel         # Arch Linux
```

#### Compiler Not Found

```bash
# Windows: Install Visual Studio Build Tools
# Download from: https://visualstudio.microsoft.com/downloads/

# macOS: Install Xcode Command Line Tools
xcode-select --install

# Linux: Install build tools
sudo apt update && sudo apt install build-essential  # Ubuntu/Debian
```

#### Build Fails with MinGW

```bash
# Ensure MinGW-w64 is installed and in PATH
# Download from: https://www.mingw-w64.org/

# Verify installation
gcc --version
g++ --version
```

#### Missing Dependencies

```bash
# Linux: Install development packages
sudo apt install libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev
```

#### Permission Issues (Linux/macOS)

```bash
# Make scripts executable
chmod +x build.sh
chmod +x build_macos.sh
```

#### Visual Studio Generator Issues

```bash
# Use specific generator
# Visual Studio 2022
cmake -G "Visual Studio 17 2022" -A x64

# Visual Studio 2019
cmake -G "Visual Studio 16 2019" -A x64
```

### Build Verification

#### Check Build Output

```bash
# Verify build files exist
ls -la build/
ls -la build/Release/  # Windows
ls -la build/          # macOS/Linux

# Check executable
file build/Release/ray.exe  # Windows
file build/ray              # macOS/Linux
```

#### Test Basic Functionality

```bash
# Quick test run
./build/ray

# Check version info (Windows)
# Right-click executable → Properties → Details
```

## 📁 Project Structure

```
Calculator_Raylib
├── .clang-format              # Code formatting rules
├── .github/                   # GitHub Actions workflows
│   └── workflows/
│       └── cmake-multi-platform.yml
├── CMakeLists.txt             # Enhanced CMake configuration
├── README.md                  # This file
├── build.bat                  # Windows Command Prompt build
├── build.sh                   # Linux build script
├── build_macos.sh             # macOS build script
├── generate_embedded_resources.bat # Resource embedding script
├── EMBEDDING_RESOURCES.md     # Resource embedding documentation
├── includes/                  # Header files
│   ├── button.h               # Button structure and functions
│   ├── calculator.h           # Calculator state and logic
│   ├── parser.h               # Mathematical expression parser
│   ├── embedded_resources.h   # Embedded resources header
│   ├── font_ubuntu.h          # Embedded font data
│   └── icon_calc.h            # Embedded icon data
├── raylib_v5/                 # Raylib library source
├── resource/                  # Application resources
│   ├── Ubuntu-Regular.ttf     # Application font
│   └── calc.png               # Application icon
└── src/                       # Source files
    ├── button.cpp             # Button creation and rendering with theming
    ├── calculator.cpp         # Calculator logic with error handling
    ├── main.cpp               # Main application with UI and performance metrics
    ├── parser.cpp             # Mathematical expression parser with validation
    ├── resource_exporter.cpp  # Resource embedding utility
    └── winmain.cpp            # Windows GUI entry point
```

## 🎯 Usage

### Basic Operations

- **Numbers:** Click digit buttons (0-9)
- **Operators:** +, -, ×, ÷
- **Equals:** = to calculate result
- **Clear:** C to reset
- **Backspace:** ← to delete last character (also removes entire function names)
- **Decimal:** . for floating point numbers
- **Sign:** ± to toggle positive/negative
- **ANS:** Use previous result in new calculations

### Scientific Functions

- **Trigonometric:** sin, cos, tan, asin, acos, atan
- **Logarithmic:** log (base 10), ln (natural logarithm)
- **Other:** √ (square root), ^ (power)

### Advanced Features

- **Theme Toggle:** Switch between light and dark mode with the ☀/☾ button
- **Expression Display:** Shows the full expression being built with auto-scrolling
- **Result History:** View previous calculations and their results
- **Performance Metrics:** Real-time FPS and frame time display
- **Detailed Error Messages:** Clear feedback for calculation errors (division by zero, invalid operations, etc.)
- **Responsive Design:** Adapts to window resizing
- **Keyboard Support:** Future enhancement planned

### Keyboard Shortcuts (Future)

- **Numbers:** 0-9 keys
- **Operators:** +, -, *, /
- **Equals:** Enter key
- **Clear:** Escape key

## 🔧 Customization

### Changing Themes

The calculator now supports a comprehensive theming system with light and dark modes. You can customize the themes by modifying the `Theme` struct in `src/main.cpp` and the category-based button colors in `src/button.cpp`:

```cpp
// In main.cpp, modify the Theme struct
struct Theme {
    Color bgColor;           // Background color
    Color displayBoxColor;   // Display box color
    Color textColor;         // Text color
    Color buttonColor;       // Default button color
};

// In button.cpp, modify colors for different button categories
std::unordered_map<int, Color> lightModeColors = {
    {NUMBER, LIGHTGRAY},
    {OPERATOR, SKYBLUE},
    {FUNCTION, GOLD},
    {CONTROL, ORANGE},
    {SPECIAL, GREEN}
};
```

### Changing Fonts

1. Replace `resource/Ubuntu-Regular.ttf` with your preferred font
2. Ensure the font supports your target languages
3. Rebuild the project

### Adding New Buttons

1. **Define in button.cpp:** Add new button definitions
2. **Add logic in calculator.cpp:** Implement new button functionality
3. **Update layout:** Modify grid positioning as needed

### Extending Functionality

- **Additional Scientific Functions:** Add more functions like exp, factorial, absolute value
- **Memory Functions:** Add M+, M-, MR, MC buttons
- **Persistent Settings:** Save user preferences like theme choice
- **Graphing Capabilities:** Implement basic function graphing
- **Custom Button Layout:** Allow users to customize button arrangement

## 📊 Performance Optimizations

### Build-Time Optimizations

- **Link Time Optimization (LTO):** Enabled for all release builds
- **Architecture-Specific:** Uses native CPU features when possible
- **Compiler Optimizations:** Aggressive optimization flags per compiler

### Runtime Optimizations

- **Performance Metrics Display:** Real-time FPS and frame time monitoring
- **Efficient Rendering:** Optimized drawing operations with category-based button rendering
- **Memory Management:** Minimal allocations during runtime with improved string handling
- **Expression Parsing:** Enhanced math parser with optimized error handling
- **Smart Backspace:** Efficiently removes entire function names in one operation
- **Cached Theme Colors:** Pre-calculated color values for different button categories

## 🔍 Development

### Code Style

- **Formatting:** Uses `.clang-format` for consistent style
- **Naming:** Follows C++ standard conventions
- **Comments:** Doxygen-style documentation

### Testing

- **Manual Testing:** Use provided test cases
- **Cross-Platform:** Test on all target platforms
- **Edge Cases:** Test division by zero, overflow, etc.

### Debugging

- **Debug Builds:** Use `-DCMAKE_BUILD_TYPE=Debug` for debugging
- **Address Sanitizer:** Enabled for GCC/Clang debug builds
- **Logging:** Add debug output as needed

## 🤝 Contributing

We welcome contributions! Here's how to get started:

### Getting Started

1. **Fork the repository**
2. **Clone your fork:**

   ```bash
   git clone https://github.com/YOUR_USERNAME/calculator_raylib.git
   cd calculator_raylib
   ```

3. **Create feature branch:**

   ```bash
   git checkout -b feature/your-feature-name
   ```

### Development Workflow

1. **Make changes** following the code style
2. **Test thoroughly** on your platform
3. **Commit with clear messages:**

   ```bash
   git commit -m "Add scientific calculator mode"
   ```

4. **Push to your fork:**

   ```bash
   git push origin feature/your-feature-name
   ```

5. **Create Pull Request** with detailed description

### Code Guidelines

- **Follow .clang-format** for consistent style
- **Add tests** for new features
- **Update documentation** for user-facing changes
- **Test on multiple platforms** when possible

### Reporting Issues

- **Bug Reports:** Include platform, compiler, and build steps
- **Feature Requests:** Describe use case and expected behavior
- **Performance Issues:** Provide profiling data when possible

## 📄 License

This project is licensed under the GNU General Public License v2 - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Raylib Team:** For the excellent graphics library
- **Contributors:** All who have contributed to this project
- **Community:** For testing and feedback

**Happy Calculating!** 🧮

For the most up-to-date information, always check the [latest release](https://github.com/LulzSec6824/calculator_raylib/releases).
