# Detect compiler type
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(IS_CLANG TRUE)
    set(IS_GCC FALSE)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(IS_CLANG FALSE) 
    set(IS_GCC TRUE)
endif()

# Enable ARC for Clang on macOS
if(IS_CLANG)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    )
endif()

# Optimized compiler flags for Release builds
if(IS_CLANG)
    # Clang-specific flags
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer -fstrict-aliasing")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer -fstrict-aliasing")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
elseif(IS_GCC)
    # GCC-specific flags
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer -fstrict-aliasing")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer -fstrict-aliasing")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
endif()

# On macOS, we don't want static linking
if(APPLE)
    string(REPLACE "-static-libstdc++" "" CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    string(REPLACE "-static-libgcc" "" CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
    string(REPLACE "-static" "" CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
endif()