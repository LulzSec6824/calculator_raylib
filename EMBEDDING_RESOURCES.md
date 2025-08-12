# Embedding Resources in the Calculator Application

## Overview

This document explains how resources like fonts and icons are embedded directly into the application's executable. This process is automatically handled by the build system in `Release` mode, which simplifies distribution by creating a single, self-contained executable that doesn't rely on external asset files.

## How It Works

The build system uses a custom utility, `resource_exporter`, to convert binary resource files (e.g., `.ttf`, `.png`) into C/C++ header files. These headers contain the file's data represented as a `char` array.

Here's a step-by-step breakdown of the process:

1. **Resource Detection**: The `CMakeLists.txt` file identifies the resource files that need to be embedded.
2. **Exporter Tool**: It builds a small command-line tool called `resource_exporter` from `src/resource_exporter.cpp`.
3. **Header Generation**: The build system runs `resource_exporter` for each resource, which generates a corresponding `.h` file in the `build/generated` directory. For example, `assets/font_ubuntu.ttf` becomes `build/generated/font_ubuntu.h`.
4. **Header Fixing**: A second utility, `fix_headers`, runs on the generated headers to add `#pragma once` directives, preventing multiple inclusion errors.
5. **Conditional Compilation**: The main application code uses the `RELEASE_BUILD` preprocessor macro, which is defined by CMake during `Release` builds.
    - When `RELEASE_BUILD` is defined, the application includes the generated headers and loads resources directly from the embedded byte arrays in memory.
    - In `Debug` mode (when `RELEASE_BUILD` is not defined), the application loads resources from the original files in the `assets` directory. This allows for faster iteration and debugging without needing to rebuild the entire project after a resource change.

## Build Process

To build the application with embedded resources, you must configure the project in `Release` mode. The provided build scripts (`build.bat`, `build.sh`, `build_macos.sh`) do this automatically.

For a manual build, use the following commands:

```bash
# 1. Configure the project for a Release build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 2. Build the project
cmake --build build --config Release
```

The resulting executable in the `build/Release` or `build` directory will be a standalone application with all resources included.

## Key Files

- **`CMakeLists.txt`**: Orchestrates the entire build process, including building and running the `resource_exporter` and `fix_headers` tools.
- **`src/resource_exporter.cpp`**: The source code for the utility that converts binary files to C++ headers.
- **`src/fix_generated_headers.cpp`**: The source code for the utility that adds `#pragma once` to the generated headers.
- **`src/calculator.cpp`**: Contains the application logic that conditionally loads resources from either files or memory based on the `RELEASE_BUILD` macro.

## Advantages of Embedding Resources

- **Simplified Distribution**: The application can be distributed as a single executable file.
- **Robustness**: Eliminates errors related to missing, moved, or corrupted resource files.
- **Integrity**: Prevents end-users from easily modifying or replacing application assets.
