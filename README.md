# IB9JHO - Programming for Quantitative Finance - Group Project (C++)

## Introduction
Pricing fixed strike Asian options using a variety of numerical option pricing methodologies. A suite of testing and analysis will then be performed, with a particular focus on computational complexity, accuracy in terms of comparison to analytical solutions and variance, as well as the standard testing of edge cases. The codebase is structured into separate header and source files to ensure modularity, extensibility, and maintainability.

We began with implementing the naïve method, followed by more complex methodologies such as antithetic variance reduction and various control variate techniques. The idea is to design a clear class structure so that we can focus on the quality of the object-oriented implementation by following the principles taught in the module.

This project concludes with a written report, reviewing existing literature and discussing how it relates to published work. Methodologies will be discussed, including the reasons as to why certain steps were taken when designing our class structure. We will also discuss some potential limitations of our class structure, and any scope for future extensions.

We have taken steps to ensure good coding practices were followed, which includes but is not limited to:

1. Planning each function and data member in each class before writing any code. This helped minimise the amount of time spent reformatting the classes due to design issues.
2. Testing classes individually before integrating them into the project. This made the debugging process significantly more simple and faster.
3. Each group member performed work on separate branches. A pull request was created where other members reviewed their work before being merged with the `main` branch.

## Implementation

The UML below briefly outlines the design of our codebase.

![](./assets/uml.jpeg)

Option.hpp and Option.cpp define a base class called `Option`, that represents a generic financial option. The base class includes a constructor for initializing the option parameters (strike price, expiry, and option type), as well as a pure virtual function called `payoff()` for computing the option's payoff at expiration. The class also contains accessor methods to retrieve the values of the strike, expiry, and option type.

AsianOption.hpp and AsianOption.cpp define a derived class called `AsianOption`, which inherits from the `Option` base class. The `AsianOption` class represents an Asian option, a type of financial option whose payoff depends on the average price of the underlying asset over a certain period. The constructor initializes specific parameters for an Asian option, such as averaging type (arithmetic or geometric) and the number of averaging periods. The `payoff()` function is overridden in this class to provide the Asian option-specific formula for calculating the payoff at expiration. Additionally, this class includes accessor methods for retrieving the averaging type and number of averaging periods.

PricingEngine.hpp and PricingEngine.cpp define a `PricingEngine` class that prices options using numerical methods like Monte Carlo simulations. The class contains a constructor for initializing the engine's parameters such as the number of simulations, spot price, risk-free rate, volatility, and expiry time. It also includes a `price()` method that takes an option object as input and computes the price using Monte Carlo simulations. This method relies on the virtual `payoff()` function defined in the `Option` base class to calculate the payoff for the specific option type.

main.cpp is the driver program that initializes an AsianOption object with the desired parameters, passes it to the PricingEngine instance, and outputs the calculated price.

The code follows object-oriented programming best practices, such as encapsulation, inheritance, and polymorphism. It is also well-documented and adheres to good coding standards and practices for readability and maintainability.

***
UPDATE: 12/05/23
***
# Project Structure and Testing Documentation
## Project Structure
The project has been updated to use a more modular directory structure. It now includes separate directories for source files (src), header files (include), and unit tests (tests).

The src directory contains the implementation files for the project. Each of these files has a .cpp extension.
The include directory contains the header files, which declare the classes and functions that are defined in the corresponding .cpp files. Each of these files has a .hpp extension.
The tests directory contains the Google Test unit test files. Each of these files has a .cpp extension and contains a series of tests for the corresponding file in the src directory.
The project uses CMake as its build system. The top-level CMakeLists.txt file in the project root directory coordinates the build, specifying the project name, the required C++ standard, and the directories that should be included in the build.

## Unit Testing with Google Test
The project uses the Google Test framework for unit testing. Google Test provides a series of macros that allow you to easily define tests and test cases, and includes features for test discovery, running the tests, and reporting the results.

Each unit test file in the tests directory corresponds to a source file in the src directory. It includes a series of tests that verify the correctness of the functions and classes defined in that source file.

To run the tests, build the project (e.g., by running cmake --build . from the command line in the build directory), and then execute the resulting test executable. This will run all the tests and report the results.

## Conclusion
This new project structure and testing framework make the code base more maintainable and robust. It is now easier to locate specific files, and the inclusion of unit tests will help to ensure the ongoing correctness of the code as it evolves over time.

***
UPDATE: 14/05/23
***
# Unit Testing with GoogleTest Framework

I have added an example of a unit test utilising the GoogleTest framework in tests,test_pricing_engine.cpp
This should help you understand how to work with the framework and created further tests.
Before writing the tests, the CMakeLists.txt file needed to be updated so that the tests themselves are linked the classes and functions that they are testing. These modifications are reflected in the minor updated to tests/CmakeLists.txt.

Now, some explanation of tests/test_pricing_engine.cpp to help you understand what is happening here.

1. SetUp(): This isn't a test per se, but it's important for the tests. This function is executed before each test is run. It sets up necessary objects and variables that the tests will use. In your case, you are creating AsianOption objects for a call and a put option, and you're setting up certain parameters like the spot price, risk-free rate, volatility, and number of simulations.

2. TearDown(): This function is executed after each test is run. It's used to clean up any resources that were used in the test. In your case, you're deleting the AsianOption objects that were created in the SetUp() function.

3. TestCalculatePriceNaiveCall: This test checks whether the calculatePriceNaive function of the PricingEngine class is working correctly for call options. Specifically, it checks whether the calculated price is greater than 0. It doesn't check whether the calculated price is correct, just that it's positive.

4. TestCalculatePriceNaivePut: This test is similar to the previous one, but it checks the calculatePriceNaive function for put options.

5. TestCalculatePriceAntitheticCall: This test checks whether the calculatePriceAntithetic function of the PricingEngine class is working correctly for call options. Again, it checks whether the calculated price is positive.

6. TestCalculatePriceAntitheticPut: This test checks the calculatePriceAntithetic function for put options.

These tests are basic sanity checks. They help to ensure that the pricing functions of the PricingEngine class don't produce completely unreasonable results (like negative prices). However, they don't check whether the pricing functions are actually calculating the correct option prices. For that, you would need to compare the calculated prices to some known correct values, for example, by calculating an analytical solution or comparing to the prices generated by another source such as an online Asian option calculator.

***
UPDATE: 14/05/23 (2)
***
# Analysis code

I have now added a new sub-directory to this repository called "analysis". I have also performed some minor restructuring to the repository which now builds "src" as a library which is then linked to from "analysis" and "tests". This change in build structure became sense as the project expands to contain an increasing number of sub-modules that all rely on the classes and methods contained within "src". These changes are reflected in the updates CMakeLists.txt, src/CMakeLists.txt, analysis/CMakeLists.txt and tests/CMakeLists.txt.

Since it is both difficult and breaks "best practice" to directly generate graphs in c++, I opted to construct the "analysis" module in a way that outputs the raw data in a csv file which we can then import into alternative software such as Excel and easily generate the desired illustrations.

A brief explanation of what the Analysis.cpp file is doing is found below:

1. Setting up the parameters: This includes the parameters for the Asian option (strike price, expiry time, option type, averaging type, and number of averaging periods), the parameters for the pricing calculation (spot price, risk-free rate, volatility), and the number of simulations for the Monte Carlo methods.

2. Creating an Asian option object: This object represents the option that we are pricing. We create two objects, one for a call option and one for a put option.

3. Calculating the price of the option: We use the Monte Carlo methods implemented in the PricingEngine class to calculate the price of the option. We calculate the price for both the call and put options, using both the naive and antithetic methods.

4. Writing the prices to a CSV file: Finally, we write the prices calculated for the different options and methods to a CSV file. This file can be used for further analysis, such as in Excel