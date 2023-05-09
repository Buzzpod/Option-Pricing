#ifndef PRICINGENGINE_HPP
#define PRICINGENGINE_HPP

#include "Option.hpp"

class PricingEngine {
public:
    double calculatePrice(const Option &option, double spot_price, double risk_free_rate, double volatility);

private:
    // Add any necessary helper methods for pricing calculations here
};

#endif // PRICINGENGINE_HPP
