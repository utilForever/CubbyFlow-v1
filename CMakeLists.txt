# CMake version
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Builds/CMake")

# Declare project
project(CubbyFlow)

# Set output directories
set(DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Includes
include_directories(Includes)
include_directories(Libraries)
include_directories(Libraries/googletest/googletest/include)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    include_directories(Libraries/winix)
endif()

# Compile options
include(Builds/CMake/CompileOptions.cmake)

# Build type - Release by default
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Overrides
set(CMAKE_MACOSX_RPATH ON)

# Project modules
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(BUILD_GTEST ON)
    set(BUILD_GMOCK OFF)
else()
    set(BUILD_GTEST 1)
    set(BUILD_GMOCK 0)
endif()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(Libraries/googletest EXCLUDE_FROM_ALL)
if ((CMAKE_VERSION VERSION_EQUAL 3.3) OR (CMAKE_VERSION VERSION_GREATER 3.3))
    cmake_policy(SET CMP0063 NEW)
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(BENCHMARK_ENABLE_TESTING OFF)
else()
    set(BENCHMARK_ENABLE_TESTING 0)
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
add_subdirectory(Libraries/cnpy)
add_subdirectory(Libraries/pystring)
add_subdirectory(Sources)
add_subdirectory(Tests/ManualTests)
add_subdirectory(Tests/UnitTests)
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_subdirectory(Libraries/winix)
endif()
add_subdirectory(Examples/HelloFluidSim)
add_subdirectory(Examples/HybridLiquidSim)
add_subdirectory(Examples/LevelSetLiquidSim)
add_subdirectory(Examples/Obj2Sdf)
add_subdirectory(Examples/Particles2Obj)
add_subdirectory(Examples/Particles2Xml)
add_subdirectory(Examples/SmokeSim)
add_subdirectory(Examples/SPHSim)

if (BUILD_FROM_PIP)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()