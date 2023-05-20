# Add test executables
add_executable(OptionTests test_option.cpp ../src/Option.cpp)
add_executable(AsianOptionTests test_asian_option.cpp ../src/AsianOption.cpp ../src/Option.cpp)
add_executable(PricingEngineTests test_pricing_engine.cpp ../src/PricingEngine.cpp ../src/AsianOption.cpp ../src/Option.cpp)

# Link test executables against gtest & gtest_main
target_link_libraries(OptionTests gtest_main)
target_link_libraries(AsianOptionTests gtest_main)
target_link_libraries(PricingEngineTests gtest_main)

# Include directories for header files
target_include_directories(OptionTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_include_directories(AsianOptionTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_include_directories(PricingEngineTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Add the tests
add_test(NAME OptionTests COMMAND OptionTests)
add_test(NAME AsianOptionTests COMMAND AsianOptionTests)
add_test(NAME PricingEngineTests COMMAND PricingEngineTests)
