# IB9JHO_Group_Project

This C++ program prices Asian options using object-oriented programming principles and a naive approach. The code is structured into separate header and source files to ensure modularity, extensibility, and maintainability.

Option.hpp and Option.cpp define a base class, Option, that represents a generic financial option. The base class includes a constructor for initializing the option parameters (strike price, expiry, and option type), as well as a pure virtual function payoff() for computing the option's payoff at expiration. The class also contains accessor methods to retrieve the values of the strike, expiry, and option type.

AsianOption.hpp and AsianOption.cpp define a derived class, AsianOption, which inherits from the Option base class. The AsianOption class represents an Asian option, a type of financial option whose payoff depends on the average price of the underlying asset over a certain period. The constructor initializes specific parameters for an Asian option, such as averaging type (arithmetic or geometric) and the number of averaging periods. The payoff() function is overridden in this class to provide the Asian option-specific formula for calculating the payoff at expiration. Additionally, this class includes accessor methods for retrieving the averaging type and number of averaging periods.

PricingEngine.hpp and PricingEngine.cpp define a PricingEngine class that prices options using numerical methods like Monte Carlo simulations. The class contains a constructor for initializing the engine's parameters, like the number of simulations, spot price, risk-free rate, volatility, and expiry time. It also provides a price() method that takes an option object as input and computes the price using Monte Carlo simulations. This method relies on the virtual payoff() function defined in the Option base class to calculate the payoff for the specific option type.

main.cpp is the driver program that initializes an AsianOption object with the desired parameters, passes it to the PricingEngine instance, and outputs the calculated price.

The code follows object-oriented programming best practices, such as encapsulation, inheritance, and polymorphism. It is also well-documented and adheres to good coding standards and practices for readability and maintainability.
