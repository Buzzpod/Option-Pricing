# Add executable
add_executable(IB9JHO_Group_Project main.cpp Option.cpp AsianOption.cpp PricingEngine.cpp)

# Include directories for header files
target_include_directories(IB9JHO_Group_Project PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
