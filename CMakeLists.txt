# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.14)

# Project's name
project(Group_Project)

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

# Fetch ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.85
)

# Fetch GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.4
)

# Fetch all the content
FetchContent_MakeAvailable(googletest imgui glfw)

# Add your source files directory
add_subdirectory(src)

# Add your test files directory
add_subdirectory(tests)
