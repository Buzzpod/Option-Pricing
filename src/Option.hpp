#ifndef OPTION_HPP
#define OPTION_HPP

class Option {
public:
    enum class Type { Call, Put };

    // specifying constructor with three member variables
    Option(double strike, double expiry, Type type); // member variables which store strike price, expiry date and type of option
    virtual double payoff(double underlyingPrice) const = 0; // calculates payoff given underlying price

    // accessor functions to retrieve values of member variables
    double getStrike() const;
    double getExpiry() const;
    Type getType() const;

protected:
    double strike;
    double expiry;
    Type type;
};

#endif // OPTION_HPP