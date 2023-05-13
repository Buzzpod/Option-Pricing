# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.14)

# Project's name
project(IB9JHO_Group_Project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Enable testing functionality
enable_testing()

# Include the FetchContent module
include(FetchContent)

# Attempt to download and configure GoogleTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)

# Add your source files directory
add_subdirectory(src)

# Add your test files directory
add_subdirectory(tests)
