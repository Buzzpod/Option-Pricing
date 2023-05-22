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

double volatility(double S, double t, double sigma_0) {
    // function to compute and return the local volatility
    double pi = std::acos(-1.0);
    double sigma = sigma_0 * (1 + 0.3 * std::cos(2 * pi * t)) * (1 + 0.5 * std::exp(-S / 50));
    return sigma;
}

std::tuple<std::vector<double>, std::vector<std::vector<double>>, std::vector<std::vector<double>>>
SDE_control_timestepper(double S0, double T, double r, double sigma_0, int Npaths) {
    // Simulate GBM paths
    // returns the time array and the paths

    // Use a time step of 1 day assuming 260 days/year
    int Nsteps = static_cast<int>(260 * T);

    double dt = T / Nsteps;
    std::vector<double> t(Nsteps + 1);
    std::vector<std::vector<double>> S(Nsteps + 1, std::vector<double>(Npaths));
    std::vector<std::vector<double>> Z(Nsteps + 1, std::vector<double>(Npaths));

    // Time step starting from initial condition
    for (int i = 0; i <= Nsteps; ++i) {
        t[i] = i * dt;
    }

    S[0] = std::vector<double>(Npaths, S0);
    Z[0] = std::vector<double>(Npaths, S0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0.0, 1.0);

    for (int n = 0; n < Nsteps; ++n) {
        std::vector<double> dW(Npaths);
        for (int i = 0; i < Npaths; ++i) {
            dW[i] = std::sqrt(dt) * dist(gen);
        }
        for (int i = 0; i < Npaths; ++i) {
            S[n + 1][i] = S[n][i] * (1. + r * dt + volatility(S[n][i], t[n], sigma_0) * dW[i]);
            Z[n + 1][i] = Z[n][i] * (1. + r * dt + volatility(S[0][i], 0, sigma_0) * dW[i]);
        }
    }

    return std::make_tuple(t, S, Z);
}

double normCDF(double x) {
    // function to compute and return the cumulative distribution function of standard normal distribution
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

std::vector<double> BS_call(double S0, double K, double T, double r, double sigma_0, int Npaths) {
    const std::vector<std::vector<double>> S = std::get<2>(SDE_control_timestepper(S0, T, r, sigma_0, Npaths));

    std::vector<double> result(Npaths);

    for (int i = 0; i < Npaths; ++i) {
        double d1 = (std::log(S[0][i] / K) + (r + volatility(S[0][i], 0, sigma_0) * volatility(S[0][i], 0, sigma_0) / 2) * T) /
            (volatility(S[0][i], 0, sigma_0) * std::sqrt(T));
        double d2 = d1 - volatility(S[0][i], 0, sigma_0) * std::sqrt(T);
        result[i] = S[0][i] * normalCDF(d1) - K * std::exp(-r * T) * normalCDF(d2);
    }

    return result;
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
double PricingEngine::SDE_control_variate_2(const Option& option, double S0, double K, double T, double r, double sigma_0, int Npaths) {   
    // Record time
    std::time_t start_time = std::time(nullptr);

    // Compute t, S, Z using SDE_control_timestepper function
    const auto sde_tuple = SDE_control_timestepper(S0, T, r, sigma_0, Npaths);
    auto t = std::get<0>(sde_tuple);
    auto S = std::get<1>(sde_tuple);
    auto Z = std::get<2>(sde_tuple);

    // Compute mean_ST using BS_call function
    std::vector<double> mean_ST = BS_call(S0, K, T, r, sigma_0, Npaths);

    // Compute discounted payoff for Asian call and control variate
    std::vector<double> fST(Npaths);
    std::vector<double> fZT(Npaths);
    for (int i = 0; i < Npaths; ++i) {
        if (option.getType() == Option::Type::Call) {
            fST[i] = std::exp(-r * T) * std::max(std::accumulate(S.begin(), S.end(), 0.0, [i](double sum, const std::vector<double>& path) { return sum + path[i]; }) / S.size() - K, 0.0);
            fZT[i] = std::exp(-r * T) * std::max(Z.back()[i] - K, 0.0);
        }
        else {
            // Put Option
            fST[i] = std::exp(-r * T) * std::max(K - std::accumulate(S.begin(), S.end(), 0.0, [i](double sum, const std::vector<double>& path) { return sum + path[i]; }) / S.size(), 0.0);
            fZT[i] = std::exp(-r * T) * std::max(K - Z.back()[i], 0.0);
        }
    }

    // Compute variance, covariance, and constant c
    double var_ST = calculate_variance(fZT);
    double cov_fST_fZT = calculate_covariance(fST, fZT);
    double c = (var_ST == 0.0) ? 0.0 : cov_fST_fZT / var_ST;

    // Compute f_c
    std::vector<double> f_c(Npaths);
    for (int i = 0; i < Npaths; ++i) {
        f_c[i] = fST[i] - c * (fZT[i] - mean_ST[i]);
    }

    // Compute price and variance
    double price = std::accumulate(f_c.begin(), f_c.end(), 0.0) / Npaths;
    return price;
}


