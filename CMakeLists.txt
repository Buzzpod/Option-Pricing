cmake_minimum_required(VERSION 3.24)
project(Group_Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Group_Project main.cpp Option.hpp Option.cpp AsianOption.hpp AsianOption.cpp PricingEngine.hpp PricingEngine.cpp)
