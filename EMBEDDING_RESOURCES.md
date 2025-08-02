# Embedding Resources in the Calculator Application

## Overview

This document explains how resources (fonts and images) are embedded into the executable in release mode, making distribution easier without needing to include separate resource files.

## How It Works

1. The application uses a conditional compilation approach:
   - In debug mode, resources are loaded from files as usual
   - In release mode, resources are embedded directly into the executable

2. The embedding process:
   - A resource exporter program converts font and image files to C code
   - The generated header files contain the binary data as arrays
   - The main application loads these resources from memory instead of disk

## Building with Embedded Resources

To build the application with embedded resources:

```bash
# Configure in Release mode
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release
```

The resulting executable will have all resources embedded and can be distributed as a standalone file.

## Implementation Details

- `resource_exporter.cpp`: Generates C header files with embedded resources
- `embedded_resources.h`: Provides functions to load embedded resources
- `CMakeLists.txt`: Handles the build process and defines the RELEASE_BUILD flag
- `main.cpp`: Uses conditional compilation to choose between file loading and embedded resources

## Advantages

- Single-file distribution
- No risk of missing or corrupted resource files
- Resources cannot be modified by end users
- Simpler deployment process