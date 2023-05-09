#include "Option.hpp"

Option::Option(double strike, double expiry, Type type)
        : strike(strike), expiry(expiry), type(type) {}

double Option::getStrike() const { return strike; }

double Option::getExpiry() const { return expiry; }

Option::Type Option::getType() const { return type; }
