# C++ options project

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