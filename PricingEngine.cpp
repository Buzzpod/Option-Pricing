#include <cmath>
#include <iostream>
#include <random>
#include "PricingEngine.hpp"
#include "AsianOption.hpp"

double PricingEngine::calculatePriceNaive(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations) {
    const AsianOption *asianOption = dynamic_cast<const AsianOption *>(&option);

    if (asianOption) {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution dist(0.0, 1.0);

        double dt = asianOption->getExpiry() / asianOption->getAveragingPeriods();
        double sumPayoffs = 0.0;

        for (unsigned int i = 0; i < numSimulations; ++i) {
            double avgSpot = 0.0;
            double spotPath = spot;

            for (unsigned int j = 0; j < asianOption->getAveragingPeriods(); ++j) {
                double randNormal = dist(gen);
                spotPath *= std::exp((riskFreeRate - 0.5 * volatility * volatility) * dt +
                                     volatility * std::sqrt(dt) * randNormal);
                avgSpot += spotPath;
            }

            avgSpot /= asianOption->getAveragingPeriods();
            sumPayoffs += option.payoff(avgSpot);
        }

        double price = (sumPayoffs / numSimulations) * std::exp(-riskFreeRate * option.getExpiry());
        return price;
    }

}

double PricingEngine::calculatePriceAntithetic(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations) {
    const AsianOption *asianOption = dynamic_cast<const AsianOption *>(&option);

    if (asianOption) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution  dist(0.0, 1.0);

        double dt = asianOption->getExpiry() / asianOption->getAveragingPeriods();
        double sumPayoffs = 0.0;

        for (unsigned int i = 0; i < numSimulations; ++i) {
            double avgSpot = 0.0;
            double avgSpotAntithetic = 0.0;
            double spotPath = spot;
            double spotPathAntithetic = spot;

            for (unsigned int j = 0; j < asianOption->getAveragingPeriods(); ++j) {
                double randNormal = dist(gen);

                spotPath *= std::exp((riskFreeRate - 0.5 * volatility * volatility) * dt +
                                     volatility * std::sqrt(dt) * randNormal);
                spotPathAntithetic *= std::exp((riskFreeRate - 0.5 * volatility * volatility) * dt -
                                               volatility * std::sqrt(dt) * randNormal);

                avgSpot += spotPath;
                avgSpotAntithetic += spotPathAntithetic;
            }

            avgSpot /= asianOption->getAveragingPeriods();
            avgSpotAntithetic /= asianOption->getAveragingPeriods();

            sumPayoffs += (option.payoff(avgSpot) + option.payoff(avgSpotAntithetic)) / 2.0;
        }

        double price = (sumPayoffs / numSimulations) * std::exp(-riskFreeRate * option.getExpiry());
        return price;
    }

}
