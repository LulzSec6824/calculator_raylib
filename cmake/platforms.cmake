# Initialize platform flags
set(IS_WINDOWS FALSE)
set(IS_MACOS FALSE)
set(IS_LINUX FALSE)

# Detect platform
if(WIN32)
    set(IS_WINDOWS TRUE)
elseif(APPLE)
    set(IS_MACOS TRUE)
elseif(UNIX AND NOT APPLE)
    set(IS_LINUX TRUE)
endif()

# Detect compiler type
set(COMPILER_IS_CLANG FALSE)
set(COMPILER_IS_GCC FALSE)
set(COMPILER_IS_MSVC FALSE)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(COMPILER_IS_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(COMPILER_IS_GCC TRUE)
elseif(MSVC)
    set(COMPILER_IS_MSVC TRUE)
endif()

# Platform-specific additions (not full flag sets - those are in main CMakeLists.txt)
if(IS_WINDOWS)
    # Windows-specific defines handled in main CMakeLists.txt
elseif(IS_MACOS)
    # macOS: Enable ARC for Objective-C if needed
    if(COMPILER_IS_CLANG)
        # Set later after target is defined
    endif()
elseif(IS_LINUX)
    # Linux: Add PIC for shared library compatibility
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()