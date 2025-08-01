@echo off
REM Batch script to build and run the calculator application with optimizations
REM Uses Release build type with LTO and other optimizations configured in CMakeLists.txt

echo Cleaning build directory...
if exist build rmdir /s /q build

echo Creating build directory...
mkdir build
cd build

echo Configuring with CMake (Release mode)...
cmake .. -DCMAKE_BUILD_TYPE=Release

echo Building the project...
cmake --build . --config Release

echo Running the calculator application...
.\Release\ray.exe

echo Done!