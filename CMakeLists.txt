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

# Build profiles and optimization settings
# Common flags for all compilers
set(COMMON_C_FLAGS "")
set(COMMON_CXX_FLAGS "")

# Platform-specific optimizations
if(IS_WINDOWS)
    # Windows-specific optimizations
    if(IS_MSVC)
        set(COMMON_C_FLAGS "${COMMON_C_FLAGS} /MP")
        set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /MP")
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
    else()
        # Windows/Linux Clang settings with thin LTO
        set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /D NDEBUG /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /D NDEBUG /W4")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF")
endif()

# RelWithDebInfo profile settings
if(IS_CLANG)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/O2 /Zi /D NDEBUG /W4 /MT")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /D NDEBUG /W4 /MT")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /DEBUG:FULL")
endif()

# MinSizeRel profile settings (optimized for size)
if(IS_CLANG)
    if(IS_MACOS)
        set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
        set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto")
    else()
        set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto=thin")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto=thin")
        set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto=thin")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_MINSIZEREL "/O1 /Os /GL /D NDEBUG /MT")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /Os /GL /D NDEBUG /MT")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /LTCG /OPT:REF /OPT:ICF")
endif()

# Target configuration
# In Release mode on Windows, create a GUI application (no console window)
if(IS_WINDOWS AND (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel"))
    add_executable(${PROJECT_NAME} WIN32)
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
if(IS_WINDOWS AND (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel"))
    target_sources(${PROJECT_NAME}
        PRIVATE
           src/winmain.cpp
    )
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
