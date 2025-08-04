@echo off
setlocal enabledelayedexpansion

:: Calculator Application Build Script
:: - Generates embedded resources
:: - Builds the application with optimizations
:: - Runs the application if build succeeds

echo ===== Building Calculator Application =====

:: 1. PREPARE DIRECTORIES
echo Creating necessary directories...
if not exist "includes" mkdir includes

:: Save any existing resource_exporter.exe before cleaning
set "saved_exporter=0"
if exist "build\resource_exporter.exe" (
    if not exist "temp" mkdir temp
    copy "build\resource_exporter.exe" "temp\resource_exporter.exe" >nul 2>&1
    set "saved_exporter=1"
)

:: Clean and recreate build directory
if exist build rmdir /s /q build
mkdir build

:: 2. GENERATE EMBEDDED RESOURCES
echo Generating embedded resource files...

:: Restore resource_exporter if we saved it
if "!saved_exporter!"=="1" (
    if exist "temp\resource_exporter.exe" (
        copy "temp\resource_exporter.exe" "build\resource_exporter.exe" >nul 2>&1
        rmdir /s /q temp
    )
)

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

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -DNDEBUG"
if %errorlevel% neq 0 (
    echo CMake configuration failed.
    exit /b 1
)

echo.

echo ===== Building Project =====
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b 1
)
echo Build completed successfully.

echo.

:: 4. RUN APPLICATION
if %ERRORLEVEL% EQU 0 (
    echo Build completed successfully.
    echo.
    echo ===== Running Calculator Application =====
    echo.
    .\ray.exe
) else (
    echo.
    echo Build failed with error code %ERRORLEVEL%
    echo Please fix the errors and try again
    exit /b %ERRORLEVEL%
)

echo.
echo Done!
endlocal