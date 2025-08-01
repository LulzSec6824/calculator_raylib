#!/bin/bash

# Script to build and run the calculator application with optimizations on Linux
# Uses Release build type with LTO and other optimizations configured in CMakeLists.txt

# Exit on error
set -e

echo -e "\033[1;36mCleaning build directory...\033[0m"
rm -rf build

echo -e "\033[1;36mCreating build directory...\033[0m"
mkdir -p build
cd build

echo -e "\033[1;36mConfiguring with CMake (Release mode)...\033[0m"
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

echo -e "\033[1;36mBuilding the project...\033[0m"
cmake --build . --config Release

echo -e "\033[1;32mRunning the calculator application...\033[0m"
./ray

echo -e "\033[1;32mDone!\033[0m"