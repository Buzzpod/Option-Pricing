# Add executable
add_executable(Group_Project main.cpp Option.cpp AsianOption.cpp PricingEngine.cpp)

# Link libraries
target_link_libraries(Group_Project PRIVATE gtest gtest_main imgui glfw)

# Include directories for header files
target_include_directories(Group_Project PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${googletest_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}
        ${glfw_SOURCE_DIR}/include
        ${imgui_SOURCE_DIR}/backends
        )
