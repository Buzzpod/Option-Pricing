#ifndef ASIANOPTION_HPP
#define ASIANOPTION_HPP

#include "Option.hpp"

class AsianOption : public Option {
public:
    enum class AveragingType { Arithmetic, Geometric };

    AsianOption(double strike, double expiry, Option::Type optionType, AveragingType averagingType, unsigned int averagingPeriods);

    double payoff(double averagePrice) const override;
    AveragingType getAveragingType() const;
    unsigned int getAveragingPeriods() const;

private:
    AveragingType averagingType;
    unsigned int averagingPeriods;
};

#endif // ASIANOPTION_HPP
