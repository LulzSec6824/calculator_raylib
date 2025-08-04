#!/bin/bash

# Calculator Application Build Script
# - Generates embedded resources
# - Builds the application with optimizations
# - Runs the application if build succeeds

set -e

echo "===== Building Calculator Application ====="

# 1. PREPARE DIRECTORIES
echo "Creating necessary directories..."
mkdir -p includes

# Save any existing resource_exporter before cleaning
saved_exporter=0
if [ -f "build/resource_exporter" ]; then
    mkdir -p temp
    cp "build/resource_exporter" "temp/resource_exporter"
    saved_exporter=1
fi

# Clean and recreate build directory
rm -rf build
mkdir -p build

# 2. GENERATE EMBEDDED RESOURCES
echo "Generating embedded resource files..."

# Restore resource_exporter if we saved it
if [ "$saved_exporter" -eq 1 ]; then
    if [ -f "temp/resource_exporter" ]; then
        cp "temp/resource_exporter" "build/resource_exporter"
        rm -rf temp
    fi
fi

# Generate resources if exporter exists, otherwise it will be built later
if [ -f "build/resource_exporter" ]; then
    echo "Running resource exporter..."
    ./build/resource_exporter
    echo "Done generating embedded resources!"
else
    echo "Resource exporter will be built during the main build process"
fi

# 3. BUILD APPLICATION
echo "Configuring with CMake (Release mode)..."
cd build

cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG"
echo

echo "===== Building Project ====="
cmake --build . --config Release

# 4. RUN APPLICATION
echo "Build completed successfully."
echo
echo "===== Running Calculator Application ====="
echo
./build/ray