#pragma once

#include "Option.hpp"

class PricingEngine {
public:
    static double calculatePriceNaive(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
    static double calculatePriceAntithetic(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
    static double calculatePriceGBM(const Option& option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations);
    static double SDE_control_variate_2(const Option& option, double spot, double strike, double riskFreeRate, double volatility, unsigned int numSimulations);
};
