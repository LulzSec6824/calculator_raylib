add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_USE_MATH_DEFINES)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(IS_CLANG TRUE)
    # Common warning flags
    set(COMMON_WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wno-missing-field-initializers -Wno-unused-function")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_WARNING_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNING_FLAGS}")

    # CPU architecture optimization
    option(USE_NATIVE_ARCH "Use -march=native for Clang (may not be portable)" ON)
    if(USE_NATIVE_ARCH)
        set(ARCH_FLAGS "-march=native -mtune=native")
    else()
        set(ARCH_FLAGS "-msse2")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARCH_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCH_FLAGS}")

    # Fast math and LTO options
    option(USE_FAST_MATH "Use -ffast-math for Clang (may affect precision)" ON)
    if(USE_FAST_MATH)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
    endif()

    option(USE_LTO "Use Link Time Optimization" ON)
    if(USE_LTO)
        set(LTO_FLAGS "-flto=thin")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${LTO_FLAGS}")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${LTO_FLAGS}")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${LTO_FLAGS}")
    endif()

    # Debug configuration
    set(DEBUG_FLAGS "-g -O0 ${COMMON_WARNING_FLAGS} -fsanitize=address")
    set(CMAKE_C_FLAGS_DEBUG "${DEBUG_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "${DEBUG_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address -static-libstdc++ -static-libgcc")

    # Release configuration
    set(RELEASE_FLAGS "-O3 -DNDEBUG ${LTO_FLAGS} ${ARCH_FLAGS} -ffast-math -fomit-frame-pointer")
    set(CMAKE_C_FLAGS_RELEASE "${RELEASE_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${RELEASE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${LTO_FLAGS} -static-libstdc++ -static-libgcc")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC specific flags
    set(COMMON_WARNING_FLAGS "-Wall -Wextra -Wpedantic -Wno-missing-field-initializers")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_WARNING_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARNING_FLAGS}")

    option(USE_NATIVE_ARCH "Use -march=native for GCC" ON)
    if(USE_NATIVE_ARCH)
        set(ARCH_FLAGS "-march=native -mtune=native")
    else()
        set(ARCH_FLAGS "-msse2")
    endif()
    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARCH_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCH_FLAGS}")

    # Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 ${COMMON_WARNING_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 ${COMMON_WARNING_FLAGS} -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address -static-libstdc++ -static-libgcc")

    # Release configuration
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -ffast-math -fomit-frame-pointer ${ARCH_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -ffast-math -fomit-frame-pointer ${ARCH_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto -static-libstdc++ -static-libgcc")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Use static runtime
    foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()

    # Enable multi-processor compilation
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /W4")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4")

    # CPU architecture optimization
    option(USE_AVX2 "Use AVX2 instruction set if available" ON)
    if(USE_AVX2)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
    endif()

    # Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "/Od /Zi /RTC1 /MTd /D_DEBUG /W4 /Gm-")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /RTC1 /MTd /D_DEBUG /W4 /Gm-")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/DEBUG:FULL /INCREMENTAL:NO")

    # Release configuration
    set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /Qpar /fp:fast /DNDEBUG /W4 /MP")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /Qpar /fp:fast /DNDEBUG /W4 /MP")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /OPT:REF /OPT:ICF /INCREMENTAL:NO /CGTHREADS:8")
endif()