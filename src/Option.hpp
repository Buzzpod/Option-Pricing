#ifndef OPTION_HPP
#define OPTION_HPP

class Option {
public:
    enum class Type { Call, Put };

    Option(double strike, double expiry, Type type);
    virtual double payoff(double underlyingPrice) const = 0;

    double getStrike() const;
    double getExpiry() const;
    Type getType() const;

protected:
    double strike;
    double expiry;
    Type type;
};

#endif // OPTION_HPP
