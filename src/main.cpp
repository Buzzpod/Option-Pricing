#include <iostream>
#include "Option.hpp"
#include "AsianOption.hpp"
#include "PricingEngine.hpp"

int main() {
    double spot_price = 100;
    double strike_price = 105;
    double risk_free_rate = 0.05;
    double volatility = 0.20;
    double expiry_time = 1.0;
    unsigned int num_simulations = 10000;
    AsianOption::AveragingType averagingType = AsianOption::AveragingType::Arithmetic;
    unsigned int averagingPeriods = 10;

    AsianOption callOption(strike_price, expiry_time, Option::Type::Call, averagingType, averagingPeriods);
    AsianOption putOption(strike_price, expiry_time, Option::Type::Put, averagingType, averagingPeriods);

    double callPriceNaive = PricingEngine::calculatePriceNaive(callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double putPriceNaive = PricingEngine::calculatePriceNaive(putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double callPriceAntithetic = PricingEngine::calculatePriceAntithetic(callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double putPriceAntithetic = PricingEngine::calculatePriceAntithetic(putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double callPriceGBM = PricingEngine::calculatePriceGBM(callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double putPriceGBM = PricingEngine::calculatePriceGBM(putOption, spot_price, risk_free_rate, volatility, num_simulations);

    std::cout << "Naive Method:" << std::endl;
    std::cout << "Call Option Price: " << callPriceNaive << std::endl;
    std::cout << "Put Option Price: " << putPriceNaive << std::endl;
    std::cout << "Antithetic Variance Reduction Method:" << std::endl;
    std::cout << "Call Option Price: " << callPriceAntithetic << std::endl;
    std::cout << "Put Option Price: " << putPriceAntithetic << std::endl;
    std::cout << "GBM Approximation Method:" << std::endl;
    std::cout << "Call Option Price: " << callPriceGBM << std::endl;
    std::cout << "Put Option Price: " << putPriceGBM << std::endl;

    return 0;
}
