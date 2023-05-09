#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include "PricingEngine.hpp"
#include "AsianOption.hpp"

double generateRandomPrice(double currentPrice, double drift, double volatility,
                           double deltaTime, std::mt19937 &randomEngine) {
    std::normal_distribution<double> normalDist(0, 1);
    double dWt = normalDist(randomEngine);
    return currentPrice * std::exp((drift - 0.5 * volatility * volatility) * deltaTime +
                                   volatility * std::sqrt(deltaTime) * dWt);
}

double PricingEngine::calculatePrice(const Option &option, double spot_price, double risk_free_rate, double volatility) {
    const AsianOption *asianOption = dynamic_cast<const AsianOption *>(&option);

    if (asianOption) {
        double deltaTime = asianOption->getExpiry() / asianOption->getAveragingPeriods();
        unsigned int numSimulations = 100000;
        double sumPayoffs = 0.0;

        std::mt19937 randomEngine(static_cast<long unsigned int>(std::time(0)));

        for (unsigned int i = 0; i < numSimulations; ++i) {
            double sumPrices = 0.0;
            double price = spot_price;

            for (unsigned int j = 0; j < asianOption->getAveragingPeriods(); ++j) {
                price = generateRandomPrice(price, risk_free_rate, volatility, deltaTime, randomEngine);
                sumPrices += price;
            }

            double averagePrice = sumPrices / asianOption->getAveragingPeriods();
            sumPayoffs += asianOption->payoff(averagePrice);
        }

        double optionPrice = (sumPayoffs / numSimulations) * std::exp(-risk_free_rate * asianOption->getExpiry());
        return optionPrice;
    } else {
        std::cout << "Unsupported option type" << std::endl;
        return 0.0;
    }
}
