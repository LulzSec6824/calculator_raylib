# Linux-specific settings
if(COMPILER_IS_CLANG)
    # Base flags for both C and C++
    set(BASE_FLAGS "-Wall -Wextra -Wpedantic -Werror -fPIC -pipe")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BASE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASE_FLAGS} -std=c++11")

    # Optimization options
    option(USE_NATIVE_ARCH "Use -march=native for Clang (may not be portable)" ON)
    if(USE_NATIVE_ARCH)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
    endif()

    option(USE_FAST_MATH "Use -ffast-math for Clang (may affect precision)" ON)
    if(USE_FAST_MATH)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
    endif()

    # LTO configuration
    option(USE_LTO "Use Link Time Optimization for Clang" ON)
    if(USE_LTO)
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto=thin")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto=thin")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
    endif()

    # Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 ${BASE_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 ${BASE_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")

    # Release configuration
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -fomit-frame-pointer")

elseif(COMPILER_IS_GCC)
    # Base flags for both C and C++
    set(BASE_FLAGS "-Wall -Wextra -Wpedantic -Werror -fPIC -pipe")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BASE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BASE_FLAGS} -std=c++11")

    # Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 ${BASE_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")
    set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 ${BASE_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")

    # Release configuration with aggressive optimizations
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
endif()