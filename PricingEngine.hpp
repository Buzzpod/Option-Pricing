#pragma once

#include "Option.hpp"

class PricingEngine {
public:
    static double calculatePriceNaive(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
    static double calculatePriceAntithetic(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
};

