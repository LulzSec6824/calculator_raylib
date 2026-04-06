cmake_minimum_required(VERSION 3.16)
project(ray LANGUAGES C CXX)

# ============================================================================
# C++20 Standard Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Build Type Configuration
# ============================================================================
# Available build types:
#   - Debug: No optimization, debug symbols, sanitizers
#   - Release: Standard release with some optimization
#   - ReleaseLTO: Full optimization with LTO (max performance)
#   - ReleaseLTOMinSize: LTO with size optimization (smallest binary)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
    "Debug" "Release" "ReleaseLTO" "ReleaseLTOMinSize")

# ============================================================================
# LTO/IPO Support Detection
# ============================================================================
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_OUTPUT)

option(ENABLE_LTO "Enable Link-Time Optimization when supported" ON)
option(USE_NATIVE_ARCH "Use -march=native (not portable)" ON)
option(USE_FAST_MATH "Use fast math optimizations (may affect precision)" ON)

# ============================================================================
# Platform Detection
# ============================================================================
include(cmake/platforms.cmake)

# ============================================================================
# Compiler-Specific Optimization Flags
# ============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Common warning flags
    set(WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wno-missing-field-initializers")
    
    # Architecture flags
    if(USE_NATIVE_ARCH AND NOT CMAKE_CROSSCOMPILING)
        set(ARCH_FLAGS "-march=native -mtune=native")
    else()
        set(ARCH_FLAGS "-msse2")
    endif()
    
    # Fast math flags
    if(USE_FAST_MATH)
        set(FAST_MATH_FLAGS "-ffast-math")
    else()
        set(FAST_MATH_FLAGS "")
    endif()
    
    # LTO flags (Clang uses thin LTO for faster linking)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(LTO_FLAGS "-flto=thin")
    else()
        set(LTO_FLAGS "-flto")
    endif()
    
    # Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 ${WARNING_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 ${WARNING_FLAGS} -fno-omit-frame-pointer")
    
    # Release configuration (standard)
    set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG ${WARNING_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG ${WARNING_FLAGS}")
    
    # ReleaseLTO configuration (max performance)
    set(CMAKE_C_FLAGS_RELEASELTO "-O3 -DNDEBUG ${WARNING_FLAGS} ${ARCH_FLAGS} ${FAST_MATH_FLAGS} ${LTO_FLAGS} -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASELTO "-O3 -DNDEBUG ${WARNING_FLAGS} ${ARCH_FLAGS} ${FAST_MATH_FLAGS} ${LTO_FLAGS} -fomit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASELTO "${LTO_FLAGS} -Wl,--gc-sections")
    
    # ReleaseLTOMinSize configuration (smallest binary)
    # Note: keeping exceptions enabled as the parser uses them for error handling
    set(CMAKE_C_FLAGS_RELEASELTOMINSIZE "-Os -DNDEBUG ${WARNING_FLAGS} ${LTO_FLAGS} -ffunction-sections -fdata-sections -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASELTOMINSIZE "-Os -DNDEBUG ${WARNING_FLAGS} ${LTO_FLAGS} -ffunction-sections -fdata-sections -fomit-frame-pointer -fno-rtti")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASELTOMINSIZE "${LTO_FLAGS} -Wl,--gc-sections -Wl,-s")
    
    # macOS doesn't support --gc-sections, use different flags
    if(APPLE)
        set(CMAKE_EXE_LINKER_FLAGS_RELEASELTO "${LTO_FLAGS} -Wl,-dead_strip")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASELTOMINSIZE "${LTO_FLAGS} -Wl,-dead_strip")
    endif()

elseif(MSVC)
    # Static runtime
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
    # Warning flags
    set(WARNING_FLAGS "/W4")
    
    # Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "/Od /Zi /RTC1 ${WARNING_FLAGS} /D_DEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /RTC1 ${WARNING_FLAGS} /D_DEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FULL /INCREMENTAL:NO")
    
    # Release configuration (standard)
    set(CMAKE_C_FLAGS_RELEASE "/O2 ${WARNING_FLAGS} /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 ${WARNING_FLAGS} /DNDEBUG")
    
    # ReleaseLTO configuration (max performance with LTCG)
    set(CMAKE_C_FLAGS_RELEASELTO "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /fp:fast ${WARNING_FLAGS} /DNDEBUG /MP")
    set(CMAKE_CXX_FLAGS_RELEASELTO "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /fp:fast ${WARNING_FLAGS} /DNDEBUG /MP")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASELTO "/LTCG /OPT:REF /OPT:ICF /INCREMENTAL:NO")
    
    # ReleaseLTOMinSize configuration (smallest binary with LTCG)
    set(CMAKE_C_FLAGS_RELEASELTOMINSIZE "/O1 /Ob1 /GL /Gy /GF ${WARNING_FLAGS} /DNDEBUG /MP")
    set(CMAKE_CXX_FLAGS_RELEASELTOMINSIZE "/O1 /Ob1 /GL /Gy /GF ${WARNING_FLAGS} /DNDEBUG /MP /GR-")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASELTOMINSIZE "/LTCG /OPT:REF /OPT:ICF /INCREMENTAL:NO")
    
    # Common MSVC defines
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_USE_MATH_DEFINES)
endif()

# ============================================================================
# Raylib Subdirectory
# ============================================================================
add_subdirectory(raylib)

# ============================================================================
# Resources
# ============================================================================
include(cmake/resources.cmake)

# ============================================================================
# Main Executable
# ============================================================================
set(CALCULATOR_SOURCES
    src/main.cpp
    src/calculator.cpp
    src/button.cpp
    src/parser.cpp
    src/metrics.cpp
    src/theme.cpp
    src/display.cpp
)

# Windows-specific: add WinMain and WIN32 subsystem for release builds
if(IS_WINDOWS)
    list(APPEND CALCULATOR_SOURCES src/winmain.cpp)
    
    if(CMAKE_CONFIGURATION_TYPES)
        # Multi-config generator (Visual Studio)
        add_executable(${PROJECT_NAME} WIN32 ${CALCULATOR_SOURCES} ${EMBEDDED_RESOURCE_FILES})
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        )
    else()
        # Single-config generator
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_executable(${PROJECT_NAME} ${CALCULATOR_SOURCES} ${EMBEDDED_RESOURCE_FILES})
        else()
            add_executable(${PROJECT_NAME} WIN32 ${CALCULATOR_SOURCES} ${EMBEDDED_RESOURCE_FILES})
        endif()
    endif()
else()
    add_executable(${PROJECT_NAME} ${CALCULATOR_SOURCES} ${EMBEDDED_RESOURCE_FILES})
endif()

# ============================================================================
# Target Configuration
# ============================================================================
add_dependencies(${PROJECT_NAME} generate_resources fix_headers_target)

target_include_directories(${PROJECT_NAME} PRIVATE includes)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Link math library on Unix-like systems
if(NOT IS_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

# ============================================================================
# Enable IPO/LTO for ReleaseLTO builds if supported
# ============================================================================
if(IPO_SUPPORTED AND ENABLE_LTO)
    set_property(TARGET ${PROJECT_NAME} PROPERTY 
        INTERPROCEDURAL_OPTIMIZATION_RELEASELTO TRUE)
    set_property(TARGET ${PROJECT_NAME} PROPERTY 
        INTERPROCEDURAL_OPTIMIZATION_RELEASELTOMINSIZE TRUE)
endif()

# ============================================================================
# C++20 Feature Requirements
# ============================================================================
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# ============================================================================
# Build Summary
# ============================================================================
message(STATUS "")
message(STATUS "=== Calculator Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "IPO/LTO Supported: ${IPO_SUPPORTED}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "======================================")