#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include "PricingEngine.hpp"
#include "AsianOption.hpp"

double PricingEngine::calculatePriceNaive(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations) {
    const AsianOption *asianOption = dynamic_cast<const AsianOption *>(&option);

    if (asianOption) {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        double dt = asianOption->getExpiry() / asianOption->getAveragingPeriods();
        double sumPayoffs = 0.0;

        for (unsigned int i = 0; i < numSimulations; ++i) {
            double avgSpot = 0.0;

            for (unsigned int j = 0; j < asianOption->getAveragingPeriods(); ++j) {
                double randNormal = dist(gen);
                double spotPath = spot * std::exp((riskFreeRate - 0.5 * volatility * volatility) * (j * dt) +
                                                  volatility * std::sqrt(j * dt) * randNormal);
                avgSpot += spotPath;
            }

            avgSpot /= static_cast<double>(asianOption->getAveragingPeriods());
            sumPayoffs += option.payoff(avgSpot);
        }

        double price = (sumPayoffs / static_cast<double>(numSimulations)) * std::exp(-riskFreeRate * option.getExpiry());
        return price;
    }

}

double PricingEngine::calculatePriceAntithetic(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations) {
    const AsianOption *asianOption = dynamic_cast<const AsianOption *>(&option);

    if (asianOption) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        double dt = asianOption->getExpiry() / asianOption->getAveragingPeriods();
        double sumPayoffs = 0.0;

        for (unsigned int i = 0; i < numSimulations; ++i) {
            double avgSpot = 0.0;
            double avgSpotAntithetic = 0.0;

            for (unsigned int j = 0; j < asianOption->getAveragingPeriods(); ++j) {
                double randNormal = dist(gen);
                double spotPath = spot * std::exp((riskFreeRate - 0.5 * volatility * volatility) * (j * dt) +
                                                  volatility * std::sqrt(j * dt) * randNormal);
                double spotPathAntithetic = spot * std::exp((riskFreeRate - 0.5 * volatility * volatility) * (j * dt) -
                                                            volatility * std::sqrt(j * dt) * randNormal);

                avgSpot += spotPath;
                avgSpotAntithetic += spotPathAntithetic;
            }

            avgSpot /= static_cast<double>(asianOption->getAveragingPeriods());
            avgSpotAntithetic /= static_cast<double>(asianOption->getAveragingPeriods());

            sumPayoffs += (option.payoff(avgSpot) + option.payoff(avgSpotAntithetic)) / 2.0;
        }

        double price = (sumPayoffs / static_cast<double>(numSimulations)) * std::exp(-riskFreeRate * option.getExpiry());
        return price;
    }

}
