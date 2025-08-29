@echo off
setlocal enabledelayedexpansion

:: Calculator Application Build Script
:: - Generates embedded resources
:: - Builds the application with optimizations
:: - Runs the application if build succeeds

echo ===== Building Calculator Application =====
:: Clean and recreate build directory
if exist build rmdir /s /q build
mkdir build

:: Generate resources if exporter exists, otherwise it will be built later
if exist "build\resource_exporter.exe" (
    echo Running resource exporter...
    build\resource_exporter.exe
    echo Done generating embedded resources!
) else (
    echo Resource exporter will be built during the main build process
)

:: 3. BUILD APPLICATION
echo Configuring with CMake (Release mode)...
cd build

:: Check if MinGW Makefiles generator is available
cmake --help | findstr "MinGW Makefiles" > nul
if %errorlevel% equ 0 (
    cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG -std=c++14"
) else (
    cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG -std=c++14"
)
if %errorlevel% neq 0 (
    echo CMake configuration failed.
    exit /b 1
)

echo ===== Building Project =====
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b 1
)
echo Build completed successfully.

:: 4. RUN APPLICATION
if %ERRORLEVEL% EQU 0 (
    echo Build completed successfully.
    echo.
    echo ===== Running Calculator Application =====
    echo.
    .\ray.exe
) else (
    echo.
    echo Build failed with error code !errorlevel!
    echo Please fix the errors and try again
    cd ..
    exit /b !errorlevel!
)

echo Done!
endlocal