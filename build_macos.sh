#!/bin/bash

# Calculator Application Build Script
# - Generates embedded resources
# - Builds the application with optimizations
# - Runs the application if build succeeds

# Exit on any error
set -e
echo "===== Building Calculator Application ====="

echo "Creating necessary directories..."
# Clean and recreate build directory
rm -rf build
mkdir -p build

# Generate resources if exporter exists, otherwise it will be built later
if [ -f "build/resource_exporter" ]; then
    echo "Running resource exporter..."
    if ! ./build/resource_exporter; then
        echo "Error: Resource exporter failed"
        exit 1
    fi
    echo "Done generating embedded resources!"
else
    echo "Resource exporter will be built during the main build process"
fi

# 3. BUILD APPLICATION
echo "Configuring with CMake (Release mode)..."
if ! cd build; then
    echo "Error: Failed to change directory to build"
    exit 1
fi

# Check if MinGW Makefiles generator is available
if cmake --help | grep -q "MinGW Makefiles"; then
    generator="MinGW Makefiles"
else
    generator="Unix Makefiles"
fi

if ! cmake .. -G "$generator" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG"; then
    echo "Error: CMake configuration failed"
    exit 1
fi
echo

echo "===== Building Project ====="
if ! cmake --build . --config Release; then
    echo "Error: Build failed"
    exit 1
fi

# 4. RUN APPLICATION
echo "Build completed successfully."
echo
echo "===== Running Calculator Application ====="
echo

# Check if executable exists before running
if [ ! -f "./ray" ]; then
    echo "Error: Executable 'ray' not found"
    exit 1
fi

./ray