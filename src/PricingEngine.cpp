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

double PricingEngine::calculatePriceGBM(const Option& option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations) {
    const AsianOption* asianOption = dynamic_cast<const AsianOption*>(&option);

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
                    double drift = (riskFreeRate - 0.5 * volatility * volatility) * dt;
                    double diffusion = volatility * std::sqrt(dt) * randNormal;
                    spotPath *= std::exp(drift + diffusion);
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

double normalCDF(double x) {
        return std::erfc(-x / std::sqrt(2)) / 2;
    }

void price_approximation_Arithmetic_Asian_Option_Black_Scholes(double spot, double riskFreeRate, double volatility, double strike_price, double expiry_time, unsigned int numSimulations, double t_1, double dt, double* call, double* put) {
    double volatility_x = volatility*std::sqrt(t_1+dt*(numSimulations-1)*(2*numSimulations-1)/6*numSimulations);
    double mu = std::log(spot)+(riskFreeRate-(volatility*volatility)/2)*(t_1+(numSimulations-1)*dt/2);
    double K_hat = 0.0;
    double pricing_sum = 0.0;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::normal_distribution<double> distribution(0.0, 1.0);  

    for (int i = 1; i <= numSimulations; ++i) {
        double mu_i = std::log(spot)+(riskFreeRate-(volatility*volatility)/2)*(t_1+(i-1)*dt);
        double volatility_i = volatility*std::sqrt(t_1+(i-1)*dt);
        double volatility_xi = volatility*volatility*(t_1+dt*((i-1)-i*(i-1)));
        K_hat += (1/numSimulations)*std::exp(mu_i+((volatility_xi*(std::log(strike_price)-mu))/(volatility_x*volatility_x))+(((volatility_i*volatility_i)-((volatility_xi*volatility_xi)/(volatility_x*volatility_x)))/2));
        pricing_sum += (1/numSimulations)*std::exp(mu_i+(volatility_i*volatility_i)/2)*normalCDF(((mu-std::log(2*strike_price-K_hat))/volatility_x)+volatility_xi/volatility_x);
    }
    *call = std::exp(-riskFreeRate*expiry_time)*(pricing_sum-strike_price*normalCDF(((mu-std::log(2*strike_price-K_hat))/volatility_x)));
    *put = std::exp(-riskFreeRate*expiry_time)*(strike_price*normalCDF(((mu-std::log(2*strike_price-K_hat))/volatility_x))-pricing_sum);
}


