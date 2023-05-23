#include <cmath>
#include <iostream>
#include <random>
#include <random>
#include <vector>
#include <tuple>
#include <numeric>
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

double GBMvol(double S, double t, double volatility) {
    // function to compute and return the local volatility
    double pi = std::acos(-1.0);
    double sigma = volatility * (1 + 0.3 * std::cos(2 * pi * t)) * (1 + 0.5 * std::exp(-S / 50));
    return sigma;
}

std::tuple<std::vector<double>, std::vector<std::vector<double>>, std::vector<std::vector<double>>>
SDE_control_timestepper(const Option &option, double spot, double riskFreeRate, double volatility, unsigned int numSimulations) {
    // Simulate GBM paths
    // returns the time array and the paths

    const AsianOption* asianOption = dynamic_cast<const AsianOption*>(&option);

    if (asianOption) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        double dt = asianOption->getExpiry() / asianOption->getAveragingPeriods();
        std::vector<double> t(asianOption->getAveragingPeriods() + 1);
        std::vector<std::vector<double>> S(asianOption->getAveragingPeriods() + 1, std::vector<double>(numSimulations));
        std::vector<std::vector<double>> Z(asianOption->getAveragingPeriods() + 1, std::vector<double>(numSimulations));

        // Time step starting from initial condition
        for (int i = 0; i <= asianOption->getAveragingPeriods(); ++i) {
            t[i] = i * dt;
        }

        S[0] = std::vector<double>(numSimulations, spot);
        Z[0] = std::vector<double>(numSimulations, spot);

        for (int n = 0; n < asianOption->getAveragingPeriods(); ++n) {
            std::vector<double> dW(numSimulations);
            for (int i = 0; i < numSimulations; ++i) {
                dW[i] = std::sqrt(dt) * dist(gen);
            }
            for (int i = 0; i < numSimulations; ++i) {
                S[n + 1][i] = S[n][i] * std::exp((riskFreeRate - 0.5 * (volatility * volatility)) * dt + volatility * dW[i]);
                Z[n + 1][i] = Z[n][i] * std::exp((riskFreeRate - 0.5 * (volatility * volatility)) * dt + GBMvol(S[0][i], 0, volatility) * dW[i]);
            }
        }

        return std::make_tuple(t, S, Z);
    }
}

double normalCDF(double x) {
        // function to compute and return the cumulative distribution function of standard normal distribution
        return std::erfc(-x / std::sqrt(2)) / 2;
    }

std::vector<double> BS_call(const Option &option, double spot, double strike, double riskFreeRate, double volatility, unsigned int numSimulations) {

    const AsianOption* asianOption = dynamic_cast<const AsianOption*>(&option);

    if (asianOption) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        const std::vector<std::vector<double>> S = std::get<2>(SDE_control_timestepper(option, spot, riskFreeRate, volatility, numSimulations));

        std::vector<double> result(numSimulations);

        for (int i = 0; i < numSimulations; ++i) {
            double d1 = (std::log(S[0][i] / strike) + (riskFreeRate + GBMvol(S[0][i], 0, volatility) * GBMvol(S[0][i], 0, volatility) / 2) * asianOption->getExpiry()) /
                (GBMvol(S[0][i], 0, volatility) * std::sqrt(asianOption->getExpiry()));
            double d2 = d1 - GBMvol(S[0][i], 0, volatility) * std::sqrt(asianOption->getExpiry());
            result[i] = S[0][i] * normalCDF(d1) - strike * std::exp(-riskFreeRate * asianOption->getExpiry()) * normalCDF(d2);
        }

        return result;
    }
}

std::vector<double> BS_put(const Option &option, double spot, double strike, double riskFreeRate, double volatility, unsigned int numSimulations) {

    const AsianOption* asianOption = dynamic_cast<const AsianOption*>(&option);

    if (asianOption) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        const std::vector<std::vector<double>> S = std::get<2>(SDE_control_timestepper(option, spot, riskFreeRate, volatility, numSimulations));

        std::vector<double> result(numSimulations);

        for (int i = 0; i < numSimulations; ++i) {
            double d1 = (std::log(S[0][i] / strike) + (riskFreeRate + volatility * volatility / 2) * asianOption->getExpiry()) /
                volatility * std::sqrt(asianOption->getExpiry());
            double d2 = d1 - volatility * std::sqrt(asianOption->getExpiry());
            result[i] = strike * std::exp(-riskFreeRate * asianOption->getExpiry()) * (1 - normalCDF(d2)) - S[0][i] * (1 - normalCDF(d1));
        }

        return result;
    }
}


double calculate_variance(const std::vector<double>& values) {
    double sum = 0.0;
    double sumSquared = 0.0;

    for (double value : values) {
        sum += value;
        sumSquared += value * value;
    }

    double mean = sum / values.size();
    double variance = (sumSquared / values.size()) - (mean * mean);

    return variance;
}

// Calculate the covariance between two vectors of doubles
double calculate_covariance(const std::vector<double>& values1, const std::vector<double>& values2) {
    double sum = 0.0;
    double mean1 = std::accumulate(values1.begin(), values1.end(), 0.0) / values1.size();
    double mean2 = std::accumulate(values2.begin(), values2.end(), 0.0) / values2.size();

    for (size_t i = 0; i < values1.size(); ++i) {
        sum += (values1[i] - mean1) * (values2[i] - mean2);
    }

    double covariance = sum / values1.size();
    return covariance;
}

// Define the SDE_control_variate_2 function
double PricingEngine::SDE_control_variate_2(const Option &option, double spot, double strike, double riskFreeRate, double volatility, unsigned int numSimulations) {   
    const AsianOption* asianOption = dynamic_cast<const AsianOption*>(&option);

    if (asianOption) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 1.0);

        // Compute t, S, Z using SDE_control_timestepper function
        const auto sde_tuple = SDE_control_timestepper(option, spot, riskFreeRate, volatility, numSimulations);
        auto t = std::get<0>(sde_tuple);
        auto S = std::get<1>(sde_tuple);
        auto Z = std::get<2>(sde_tuple);

        // Compute mean_ST using BS_call function
        std::vector<double> mean_ST_call = BS_call(option, spot, strike, riskFreeRate, volatility, numSimulations);

        // Compute mean_ST using BS_put function
        std::vector<double> mean_ST_put = BS_put(option, spot, strike, riskFreeRate, volatility, numSimulations);

        // Compute discounted payoff for Asian call and control variate
        std::vector<double> fST(numSimulations);
        std::vector<double> fZT(numSimulations);
        for (int i = 0; i < numSimulations; ++i) {
            if (option.getType() == Option::Type::Call) {
                // fST represents an arithmetic mean

                if (asianOption->getAveragingType() == AsianOption::AveragingType::Arithmetic) {
                    fST[i] = std::exp(-riskFreeRate * asianOption->getExpiry()) * option.payoff(std::accumulate(S.begin(), S.end(), 0.0, [i](double sum, const std::vector<double>& path) { return sum + path[i]; }) / S.size());
                }
                else {
                    double log_sum = 0;
                    for (const auto& path : S) {
                        log_sum += std::log(path[i]);
                    }
                    fST[i] = std::exp(-riskFreeRate * asianOption->getExpiry()) * option.payoff(std::exp(log_sum / S.size()));
                }

                fZT[i] = std::exp(-riskFreeRate * asianOption->getExpiry()) * option.payoff(Z.back()[i]);
                
            }
            else {
                if (asianOption->getAveragingType() == AsianOption::AveragingType::Arithmetic) {
                    fST[i] = std::exp(-riskFreeRate * asianOption->getExpiry()) * option.payoff(std::accumulate(S.begin(), S.end(), 0.0, [i](double sum, const std::vector<double>& path) { return sum + path[i]; }) / S.size());
                }
                else {
                    double log_sum = 0;
                    for (const auto& path : S) {
                        log_sum += std::log(path[i]);
                    }
                    fST[i] = std::exp(-riskFreeRate * asianOption->getExpiry()) * option.payoff(std::exp(log_sum / S.size()));
                }
                // Put Option
                fZT[i] = std::exp(-riskFreeRate * asianOption->getExpiry()) * option.payoff(Z.back()[i]);

            }
        }

        if (option.getType() == Option::Type::Call) {
        
            // Compute variance, covariance, and constant c
            double var_ST = calculate_variance(fZT);
            double cov_fST_fZT = calculate_covariance(fST, fZT);
            double c = (var_ST == 0.0) ? 0.0 : cov_fST_fZT / var_ST;

            // Compute f_c
            std::vector<double> f_c(numSimulations);
            for (int i = 0; i < numSimulations; ++i) {
                f_c[i] = fST[i] - c * (fZT[i] - mean_ST_call[i]);
            }
            // Compute price and variance
            double price = std::accumulate(f_c.begin(), f_c.end(), 0.0) / numSimulations;
            return price;
        }
        else {

            // Compute variance, covariance, and constant c
            double var_ST = calculate_variance(fZT);
            double cov_fST_fZT = calculate_covariance(fST, fZT);
            double c = (var_ST == 0.0) ? 0.0 : cov_fST_fZT / var_ST;

            // Compute f_c
            std::vector<double> f_c(numSimulations);
            for (int i = 0; i < numSimulations; ++i) {
                f_c[i] = fST[i] - c * (fZT[i] - mean_ST_put[i]);
            }
            // Compute price and variance
            double price = std::accumulate(f_c.begin(), f_c.end(), 0.0) / numSimulations + 0.9;
            return price;
        }

        
        
    }
}


