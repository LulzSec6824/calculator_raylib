cmake_minimum_required(VERSION 3.10)
project(ray VERSION 1.0.0 LANGUAGES C CXX)

# Platform detection
set(IS_WINDOWS FALSE)
set(IS_MACOS FALSE)
set(IS_LINUX FALSE)

if(WIN32)
    set(IS_WINDOWS TRUE)
elseif(APPLE)
    set(IS_MACOS TRUE)
elseif(UNIX AND NOT APPLE)
    set(IS_LINUX TRUE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler detection
set(IS_MSVC FALSE)
set(IS_CLANG FALSE)
set(IS_GCC FALSE)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(IS_MSVC TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(IS_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(IS_GCC TRUE)
endif()

# Runtime library configuration
# For MSVC, set static runtime before adding subdirectories
if(IS_MSVC)
    # Use static runtime libraries (/MT)
    foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach(flag_var)
endif()

add_subdirectory(raylib_v5)

#------- BUILD PROFILE CONFIGS-----------
# Common flags for all compilers
set(COMMON_C_FLAGS "")
set(COMMON_CXX_FLAGS "")

# Platform-specific optimizations
if(IS_WINDOWS)
    # Windows-specific optimizations
    if(IS_MSVC)
        # Enable multi-processor compilation
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} /MP")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /MP")
        # Enable enhanced instruction sets if available
        # Use SSE2 as a safer default (supported by all 64-bit CPUs)
        set(ARCH_FLAG "/arch:SSE2")
        
        # Check if AVX2 option is explicitly requested
        option(USE_AVX2 "Use AVX2 instruction set if available" OFF)
        if(USE_AVX2)
            set(ARCH_FLAG "/arch:AVX2")
        endif()
        
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} ${ARCH_FLAG}")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} ${ARCH_FLAG}")
        # Enable whole program optimization
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} /GL")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /GL")
        # Enable intrinsic functions
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} /Oi")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /Oi")
    elseif(IS_CLANG)
        # Enable warnings
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -Wall -Wextra -Wpedantic")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -Wall -Wextra -Wpedantic")
        # Architecture optimization options
        option(USE_NATIVE_ARCH "Use -march=native for Clang (may not be portable)" OFF)
        option(USE_FAST_MATH "Use -ffast-math for Clang (may affect precision)" OFF)
        option(USE_LTO "Use Link Time Optimization for Clang" ON)
        
        if(USE_NATIVE_ARCH)
            set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -march=native -mtune=native")
            set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -march=native -mtune=native")
        else()
            # Use SSE2 as a safer default (supported by all 64-bit CPUs)
            set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -msse2")
            set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -msse2")
        endif()
        
        # Enable fast math if requested
        if(USE_FAST_MATH)
            set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -ffast-math")
            set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -ffast-math")
        endif()
        
        # Enable link-time optimization if requested
        if(USE_LTO)
            set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -flto=thin")
            set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -flto=thin")
        endif()
    endif()
elseif(IS_MACOS)
    # macOS-specific optimizations
    if(IS_CLANG)
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -mmacosx-version-min=10.13")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -mmacosx-version-min=10.13")
    endif()
elseif(IS_LINUX)
    # Linux-specific optimizations
    if(IS_GCC OR IS_CLANG)
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} -fPIC")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -fPIC")
    endif()
endif()

# Apply common flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CXX_FLAGS}")

# Debug build profile settings
if(IS_CLANG)
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_DEBUG "/Od /Zi /RTC1 /MTd /D_DEBUG /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /RTC1 /MTd /D_DEBUG /W4")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG:FULL")
endif()

# Release build profile settings with LTO
if(IS_CLANG)
    if(IS_MACOS)
        # macOS Clang settings
        set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
    elseif(IS_WINDOWS)
        # Windows Clang settings with enhanced optimizations
        set(CLANG_RELEASE_FLAGS "-O3 -DNDEBUG -fomit-frame-pointer -fvectorize -funroll-loops")
        # Add architecture flags if enabled
        if(USE_NATIVE_ARCH)
            set(CLANG_RELEASE_FLAGS "${CLANG_RELEASE_FLAGS} -march=native -mtune=native")
        endif()
        # Add fast math if enabled
        if(USE_FAST_MATH)
            set(CLANG_RELEASE_FLAGS "${CLANG_RELEASE_FLAGS} -ffast-math")
        endif()
        # Add LTO if enabled
        if(USE_LTO)
            set(CLANG_RELEASE_FLAGS "${CLANG_RELEASE_FLAGS} -flto=thin")
            set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
        endif()
        
        set(CMAKE_C_FLAGS_RELEASE "${CLANG_RELEASE_FLAGS}")
        set(CMAKE_CXX_FLAGS_RELEASE "${CLANG_RELEASE_FLAGS}")
    else()
        # Linux Clang settings with thin LTO
        set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /Qpar /fp:fast ${ARCH_FLAG} /D NDEBUG /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /Qpar /fp:fast ${ARCH_FLAG} /D NDEBUG /W4")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF /CGTHREADS:8")
endif()

# RelWithDebInfo profile settings
if(IS_CLANG)
    if(IS_WINDOWS)
        # Windows Clang settings with debug info
        set(CLANG_RELWITHDEBINFO_FLAGS "-O2 -g -DNDEBUG")
        
        # Add architecture flags if enabled
        if(USE_NATIVE_ARCH)
            set(CLANG_RELWITHDEBINFO_FLAGS "${CLANG_RELWITHDEBINFO_FLAGS} -march=native -mtune=native")
        endif()
        
        # Add fast math if enabled
        if(USE_FAST_MATH)
            set(CLANG_RELWITHDEBINFO_FLAGS "${CLANG_RELWITHDEBINFO_FLAGS} -ffast-math")
        endif()
        
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CLANG_RELWITHDEBINFO_FLAGS}")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CLANG_RELWITHDEBINFO_FLAGS}")
    else()
        # macOS/Linux Clang settings
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/O2 /Zi /Gy /GF /Oi ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /Gy /GF /Oi ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /DEBUG:FULL /OPT:REF /OPT:ICF")
endif()

# MinSizeRel profile settings (optimized for size)
if(IS_CLANG)
    if(IS_MACOS)
        set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
        set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto")
    elseif(IS_WINDOWS)
        # Windows Clang settings optimized for size
        set(CLANG_MINSIZEREL_FLAGS "-Os -DNDEBUG -fomit-frame-pointer -fvectorize")
        
        # Add LTO if enabled
        if(USE_LTO)
            set(CLANG_MINSIZEREL_FLAGS "${CLANG_MINSIZEREL_FLAGS} -flto=thin")
            set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto=thin")
        endif()
        
        set(CMAKE_C_FLAGS_MINSIZEREL "${CLANG_MINSIZEREL_FLAGS}")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CLANG_MINSIZEREL_FLAGS}")
    else()
        # Linux Clang settings with thin LTO
        set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto=thin")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto=thin")
        set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto=thin")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_MINSIZEREL "/O1 /Os /GL /Gy /GF /Oi /Gw ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /Os /GL /Gy /GF /Oi /Gw ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /LTCG /OPT:REF /OPT:ICF /CGTHREADS:8")
endif()


# Target configuration
# In Release mode on Windows, create a GUI application (no console window)
if(IS_WINDOWS)
    # Handle both single-configuration and multi-configuration generators
    if(CMAKE_CONFIGURATION_TYPES)
        # Multi-configuration generator (Visual Studio, Xcode)
        add_executable(${PROJECT_NAME} WIN32)
        # Set the SUBSYSTEM:CONSOLE property for Debug and RelWithDebInfo configurations
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
            LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE"
        )
    else()
        # Single-configuration generator
        if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
            add_executable(${PROJECT_NAME} WIN32)
        else()
            add_executable(${PROJECT_NAME})
        endif()
    endif()
else()
    add_executable(${PROJECT_NAME})
endif()

# Source files
# Common source files for all configurations
target_sources(${PROJECT_NAME}
    PRIVATE
       src/main.cpp
       src/calculator.cpp
       src/button.cpp
       src/parser.cpp
)

# Platform-specific source files
if(IS_WINDOWS)
    if(CMAKE_CONFIGURATION_TYPES)
        # For multi-configuration generators, always include winmain.cpp
        # It will only be used when the subsystem is Windows
        target_sources(${PROJECT_NAME}
            PRIVATE
               src/winmain.cpp
        )
    else()
        # For single-configuration generators, only include for GUI builds
        if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
            target_sources(${PROJECT_NAME}
                PRIVATE
                   src/winmain.cpp
            )
        endif()
    endif()
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE includes)

# Link libraries
# Main dependencies
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
)

# Add math library for non-Windows platforms
if(NOT IS_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

# Platform-specific settings
# MSVC-specific settings
if(IS_MSVC)
    # Add preprocessor definitions for MSVC compatibility
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        _USE_MATH_DEFINES
    )
endif()

# macOS-specific settings
if(IS_MACOS)
    # Enable ARC (Automatic Reference Counting) for Objective-C/C++ code
    if(IS_CLANG)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
        )
    endif()
endif()

# Resource handling
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resource)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/Ubuntu-Regular.ttf 
    ${CMAKE_CURRENT_BINARY_DIR}/resource/Ubuntu-Regular.ttf 
    COPYONLY
)

# Copy PNG icon file for all platforms and build types
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/calc.png
    ${CMAKE_CURRENT_BINARY_DIR}/resource/calc.png
    COPYONLY
)
