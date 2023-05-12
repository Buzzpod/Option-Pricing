# Add executable
add_executable(Group_Project main.cpp Option.cpp AsianOption.cpp PricingEngine.cpp)

# Include directories for header files
target_include_directories(Group_Project PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
