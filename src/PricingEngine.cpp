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
        std::normal_distribution<double> dist(0.0, 1.0);

        double dt = asianOption->getExpiry() / asianOption->getAveragingPeriods();
        double sumPayoffs = 0.0;

        for (unsigned int i = 0; i < numSimulations; ++i) {
            double sumSpot = 0.0;
            double productSpot = 1.0;
            double spotPath = spot;

            for (unsigned int j = 0; j < asianOption->getAveragingPeriods(); ++j) {
                if (j > 0) {
                    double randNormal = dist(gen);
                    spotPath *= std::exp((riskFreeRate - 0.5 * volatility * volatility) * dt +
                                         volatility * std::sqrt(dt) * randNormal);
                }
                sumSpot += spotPath;
                productSpot *= spotPath;
            }

            double avgSpot;
            if (asianOption->getAveragingType() == AsianOption::AveragingType::Arithmetic) {
                avgSpot = sumSpot / asianOption->getAveragingPeriods();
            } else { // AsianOption::Geometric
                avgSpot = std::pow(productSpot, 1.0 / asianOption->getAveragingPeriods());
            }

            sumPayoffs += option.payoff(avgSpot);
        }

        double price = (sumPayoffs / numSimulations) * std::exp(-riskFreeRate * asianOption->getExpiry());
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
            double sumSpot = 0.0;
            double sumSpotAntithetic = 0.0;
            double productSpot = 1.0;
            double productSpotAntithetic = 1.0;
            double spotPath = spot;
            double spotPathAntithetic = spot;

            for (unsigned int j = 0; j < asianOption->getAveragingPeriods(); ++j) {
                if (j > 0) {
                    double randNormal = dist(gen);
                    spotPath *= std::exp((riskFreeRate - 0.5 * volatility * volatility) * dt +
                                         volatility * std::sqrt(dt) * randNormal);
                    spotPathAntithetic *= std::exp((riskFreeRate - 0.5 * volatility * volatility) * dt -
                                                   volatility * std::sqrt(dt) * randNormal);
                }

                sumSpot += spotPath;
                sumSpotAntithetic += spotPathAntithetic;
                productSpot *= spotPath;
                productSpotAntithetic *= spotPathAntithetic;
            }

            double avgSpot, avgSpotAntithetic;
            if (asianOption->getAveragingType() == AsianOption::AveragingType::Arithmetic) {
                avgSpot = sumSpot / asianOption->getAveragingPeriods();
                avgSpotAntithetic = sumSpotAntithetic / asianOption->getAveragingPeriods();
            } else { // AsianOption::Geometric
                avgSpot = std::pow(productSpot, 1.0 / asianOption->getAveragingPeriods());
                avgSpotAntithetic = std::pow(productSpotAntithetic, 1.0 / asianOption->getAveragingPeriods());
            }

            sumPayoffs += (option.payoff(avgSpot) + option.payoff(avgSpotAntithetic)) / 2.0;
        }

        double price = (sumPayoffs / numSimulations) * std::exp(-riskFreeRate * asianOption->getExpiry());
        return price;
    }

}

