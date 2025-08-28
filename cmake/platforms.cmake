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

# Include platform-specific CMake files
if(IS_WINDOWS)
    include(${CMAKE_CURRENT_LIST_DIR}/windows.cmake)
elseif(IS_MACOS)
    include(${CMAKE_CURRENT_LIST_DIR}/macos.cmake)
elseif(IS_LINUX)
    include(${CMAKE_CURRENT_LIST_DIR}/linux.cmake)
endif()