#ifndef ASIANOPTION_HPP
#define ASIANOPTION_HPP

#include "Option.hpp"

// Defines class called AsianOption derived from Option class
class AsianOption : public Option {
public:
    enum class AveragingType { Arithmetic, Geometric };

    // Specifying constructor with three member variables from base Option class and initialises additional member variables specific to AsianOption
    AsianOption(double strike, double expiry, Option::Type optionType, AveragingType averagingType, unsigned int averagingPeriods);

    double payoff(double averagePrice) const override; // Overrides payoff() method inherited from the base Option class

    // Accessor functions to retrieve values of the member variables
    AveragingType getAveragingType() const;
    unsigned int getAveragingPeriods() const;

private:
    AveragingType averagingType;
    unsigned int averagingPeriods;
};

#endif // ASIANOPTION_HPP