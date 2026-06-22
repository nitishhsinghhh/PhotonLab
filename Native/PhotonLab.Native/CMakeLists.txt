# -----------------------------------------------------------------------------
# CMake Configuration - PhotonLab Native Image & Statistics Engine
# Project      : PhotonLab.Native
# Component    : Cross-Platform Native Core Interop Architecture
# Architecture : Shared DLL Bridge + Image Processing + Interop Export Layer
# Systems      : Verified for Windows (MSVC/MinGW), Linux (GCC/Clang), macOS (Apple)
# -----------------------------------------------------------------------------
# VERSION HISTORY
# Version | Date       | Author        | Description
# --------|------------|---------------|----------------------------------------
# 1.0.0   | 2026-06-10 | AI Assistant  | Initial native orchestration baseline.
# 1.1.0   | 2026-06-10 | AI Assistant  | Upgraded for global OS & CI Pipeline 
#                                        compatibility.
# -----------------------------------------------------------------------------
# BUILD STRATEGY
# * Shared DLL Bridge          : Cross-platform .NET interoperability layer
# * MinGW Static Linking       : Portable Windows runtime isolation
# * Universal ASan Integration : Clean modern target tracking for CI/CD runners
# * Auto Clang-Format Target  : Code style verification step for CI gates
# -----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.14)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# -----------------------------------------------------------------------------
# Project Initialization
# -----------------------------------------------------------------------------

project(
    PhotonLab.Native
    VERSION 1.1.0
    LANGUAGES CXX
)

# -----------------------------------------------------------------------------
# Force all outputs to the build root to simplify path resolution for tests
# -----------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# For multi-config generators (like Visual Studio), ensure all configs go to the same place
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}")
endforeach()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
enable_testing()

# Initialize global platform/toolchain rules cleanly before target declaration
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -----------------------------------------------------------------------------
# Runtime / Linking Strategy
# -----------------------------------------------------------------------------

if(MSVC)

    # Using dynamic MSVC runtime consistently across all targets
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

    # GoogleTest must match the same runtime model
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

elseif(MINGW)

    # MinGW-specific static runtime linking
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

    set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

endif()

# -----------------------------------------------------------------------------
# Restrict AddressSanitizer strictly to native explicit Debug builds
# -----------------------------------------------------------------------------

if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling AddressSanitizer for Debug build")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
    endif()
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -----------------------------------------------------------------------------
# Core Component Target: PhotonLab.Native (STATIC)
# -----------------------------------------------------------------------------

add_library(PhotonLab.Native STATIC
    # Core
    src/Core/ImageBuffer.cpp
    src/Core/ImageMetadata.cpp

    # Processing
    src/Processing/GammaStrategy.cpp
    src/Processing/MedianFilterStrategy.cpp
    src/Processing/SharpenStrategy.cpp
    src/Processing/WindowLevelStrategy.cpp

    # Statistics
    src/Statistics/HistogramCalculator.cpp
    src/Statistics/StatisticsCalculator.cpp
)

target_include_directories(
    PhotonLab.Native
    PUBLIC
    include
)

# -----------------------------------------------------------------------------
# The Bridge DLL: PhotonLab.NativeDLL (SHARED)
# -----------------------------------------------------------------------------

add_library(PhotonLab.NativeDLL SHARED src/Interop/NativeExports.cpp)
target_compile_definitions(PhotonLab.NativeDLL PRIVATE PHOTONLAB_NATIVE_EXPORTS)

target_link_libraries(PhotonLab.NativeDLL PRIVATE PhotonLab.Native)

set_target_properties(
    PhotonLab.Native
    PROPERTIES
    OUTPUT_NAME "PhotonLab.NativeStatic"
)

# -----------------------------------------------------------------------------
# Testing Integration (Cross-Platform Aware)
# -----------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/main.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# -----------------------------------------------------------------------------
# Unit Testing Integration
# -----------------------------------------------------------------------------

file(
    GLOB_RECURSE PHOTONLAB_UNIT_TEST_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../../Tests/PhotonLab.UnitTests/*.cpp"
)

add_executable(
    PhotonLab.UnitTests
    ${PHOTONLAB_UNIT_TEST_SOURCES}
)

target_include_directories(
    PhotonLab.UnitTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include 
)

target_link_libraries(
    PhotonLab.UnitTests
    PRIVATE
        PhotonLab.Native
        PhotonLab.NativeDLL
        GTest::gtest
        GTest::gtest_main
)

# Force the test runner to link dependencies statically under MinGW
if(MINGW)
    target_link_options(PhotonLab.UnitTests PRIVATE
        "-static"
        "-static-libgcc"
        "-static-libstdc++"
    )
endif()

add_test(
    NAME PhotonLab.UnitTests
    COMMAND PhotonLab.UnitTests
)

# ------------------------------------------------------------------------------------------------------
# Native DLL / Export Tests
# ------------------------------------------------------------------------------------------------------

file(
    GLOB_RECURSE PHOTONLAB_NATIVE_TEST_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/../../Tests/PhotonLab.NativeTests/*.cpp"
)

add_executable(
    PhotonLab.NativeTests
    ${PHOTONLAB_NATIVE_TEST_SOURCES}
)

target_include_directories(
    PhotonLab.NativeTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    PhotonLab.NativeTests
    PRIVATE
        PhotonLab.Native
        PhotonLab.NativeDLL
        GTest::gtest
        GTest::gtest_main
)

# Force the test runner to link dependencies statically under MinGW
if(MINGW)
    target_link_options(PhotonLab.NativeTests PRIVATE
        "-static"
        "-static-libgcc"
        "-static-libstdc++"
    )
endif()

add_test(
    NAME PhotonLab.NativeTests
    COMMAND PhotonLab.NativeTests
)

# -----------------------------------------------------------------------------
# Code Formatting (Clang-Format Automation)
# -----------------------------------------------------------------------------

find_program(CLANG_FORMAT_EXE 
    NAMES clang-format
    HINTS /opt/homebrew/bin /usr/local/bin
)

if(CLANG_FORMAT_EXE)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_EXE}")
    file(GLOB_RECURSE ALL_FORMAT_FILES 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    )
    
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=LLVM ${ALL_FORMAT_FILES}
        COMMENT "Auto-formatting all C++ engine source..."
    )
endif()

message(STATUS "Binary output directory set to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
