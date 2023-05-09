#include <algorithm>
#include "AsianOption.hpp"

AsianOption::AsianOption(double strike, double expiry, Option::Type optionType, AveragingType averagingType, unsigned int averagingPeriods)
        : Option(strike, expiry, optionType), averagingType(averagingType), averagingPeriods(averagingPeriods) {
}

double AsianOption::payoff(double averagePrice) const {
    if (getType() == Option::Type::Call) {
        return std::max(averagePrice - strike, 0.0);
    } else { // Put option
        return std::max(strike - averagePrice, 0.0);
    }
}

AsianOption::AveragingType AsianOption::getAveragingType() const {
    return averagingType;
}

unsigned int AsianOption::getAveragingPeriods() const {
    return averagingPeriods;
}
