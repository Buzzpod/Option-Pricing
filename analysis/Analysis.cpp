#include <fstream>
#include <iomanip>
#include "../src/PricingEngine.hpp"
#include "../src/AsianOption.hpp"
#include <chrono>

class SpotVsOptionWriter {
public:
    void writeData(const std::string& filename) {
        // Open a file in write mode
        std::ofstream outfile(filename);

        // Write the headers
        outfile << "SpotPrice,OptionPrice1,OptionPrice2,OptionPrice3,OptionPrice4\n";

        // Calculate prices for a range of spot prices and varying expiry times
        for (double spot = 20.0; spot <= 180.0; spot += 1.0) {
            AsianOption optionExpiry1(105.0, 1.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            AsianOption optionExpiry2(105.0, 4.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            AsianOption optionExpiry3(105.0, 8.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            AsianOption optionExpiry4(105.0, 12.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);

            double price1 = PricingEngine::calculatePriceGBM(optionExpiry1, spot, 0.05, 0.20, 10000);
            double price2 = PricingEngine::calculatePriceGBM(optionExpiry2, spot, 0.05, 0.20, 10000);
            double price3 = PricingEngine::calculatePriceGBM(optionExpiry3, spot, 0.05, 0.20, 10000);
            double price4 = PricingEngine::calculatePriceGBM(optionExpiry4, spot, 0.05, 0.20, 10000);

            // Write the spot price and option prices to the CSV file
            outfile << std::fixed << std::setprecision(2) << spot << "," << price1 << "," << price2 << "," << price3 << "," << price4 << "\n";
        }

        // Close the opened file
        outfile.close();
    }
};

class ConvergenceWriter {
public:
    void writeData(const std::string& filename) {
        // Open a file in write mode
        std::ofstream outfile(filename);

        // Write the headers
        outfile << "NumSimulations,PriceNaive,PriceAntithetic,PriceGBM\n";

        // Calculate prices for a range of iterations
        for (double numSimulations = 1000; numSimulations <= 1000000; numSimulations += 5000) {
            AsianOption option(105.0, 1.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            double priceNaive = PricingEngine::calculatePriceNaive(option, 100.0, 0.05, 0.20, numSimulations);
            double priceAntithetic = PricingEngine::calculatePriceAntithetic(option, 100.0, 0.05, 0.20, numSimulations);
            double priceGBM = PricingEngine::calculatePriceGBM(option, 100.0, 0.05, 0.20, numSimulations);

            // Write the number of simulations and option prices to the CSV file
            outfile << std::fixed << std::setprecision(2) << numSimulations << "," << priceNaive << "," << priceAntithetic << "," << priceGBM << "\n";
        }

        // Close the opened file
        outfile.close();
    }
};

class OptionPriceVsVolatilityWriter {
public:
    void writeData(const std::string& filename) {
        // Open a file in write mode
        std::ofstream outfile(filename);

        // Write the headers
        outfile << "Volatility,OptionPrice1,OptionPrice2,OptionPrice3,OptionPrice4\n";

        // Calculate prices for a range of volatilities while keeping other parameters constant
        for (double volatility = 0.02; volatility <= 0.80; volatility += 0.005) {
            AsianOption optionVolatility1(105.0, 1.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            AsianOption optionVolatility2(105.0, 4.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            AsianOption optionVolatility3(105.0, 8.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            AsianOption optionVolatility4(105.0, 12.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);

            double price1 = PricingEngine::calculatePriceGBM(optionVolatility1, 100.0, 0.05, volatility, 10000);
            double price2 = PricingEngine::calculatePriceGBM(optionVolatility2, 100.0, 0.05, volatility, 10000);
            double price3 = PricingEngine::calculatePriceGBM(optionVolatility3, 100.0, 0.05, volatility, 10000);
            double price4 = PricingEngine::calculatePriceGBM(optionVolatility4, 100.0, 0.05, volatility, 10000);

            // Write the volatility and option prices to the CSV file
            outfile << std::fixed << std::setprecision(3) << volatility << "," << price1 << "," << price2 << "," << price3 << "," << price4 << "\n";
        }

        // Close the opened file
        outfile.close();
    }
};



class EfficiencyWriter {
public:
    void writeData(const std::string& filename) {
        // Open a file in write mode
        std::ofstream outfile(filename);

        // Write the headers
        outfile << "NumSimulations,TimeNaive,TimeAntithetic\n";

        // Calculate prices and measure computational time for a range of iterations
        for (double numSimulations = 1000; numSimulations <= 1000000; numSimulations += 5000) {
            AsianOption option(105.0, 1.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);

            // Measure computational time for the naive method
            auto startNaive = std::chrono::high_resolution_clock::now();
            PricingEngine::calculatePriceNaive(option, 100.0, 0.05, 0.20, numSimulations);
            auto endNaive = std::chrono::high_resolution_clock::now();
            auto timeNaive = std::chrono::duration_cast<std::chrono::microseconds>(endNaive - startNaive).count();

            // Measure computational time for the antithetic method
            auto startAntithetic = std::chrono::high_resolution_clock::now();
            PricingEngine::calculatePriceAntithetic(option, 100.0, 0.05, 0.20, numSimulations);
            auto endAntithetic = std::chrono::high_resolution_clock::now();
            auto timeAntithetic = std::chrono::duration_cast<std::chrono::microseconds>(endAntithetic - startAntithetic).count();

            // Write the number of simulations and computational times to the CSV file
            outfile << std::fixed << std::setprecision(3) << numSimulations << "," << timeNaive << "," << timeAntithetic << "\n";
        }

        // Close the opened file
        outfile.close();
    }
};

class ToleranceWriter {
public:
    static void writeData(const std::string& filename) {
        // Open a file in write mode
        std::ofstream outfile(filename);

        // Write the headers
        outfile << "NumSimulations,PriceDiffNaive,PriceDiffAntithetic\n";

        // Calculate prices for a range of iterations
        for (int numSimulations = 1000; numSimulations <= 1000000; numSimulations += 5000) {
            AsianOption option(105.0, 1.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
            double priceNaive = PricingEngine::calculatePriceNaive(option, 100.0, 0.05, 0.20, numSimulations);
            double priceAntithetic = PricingEngine::calculatePriceAntithetic(option, 100.0, 0.05, 0.20, numSimulations);
            double priceGBM = PricingEngine::calculatePriceGBM(option, 100.0, 0.05, 0.20, numSimulations);

            double priceDiffNaive = std::abs(priceNaive - priceGBM);
            double priceDiffAntithetic = std::abs(priceAntithetic - priceGBM);

            // Write the number of simulations and option prices to the CSV file
            outfile << std::fixed << std::setprecision(2) << numSimulations << "," << priceDiffNaive << "," << priceDiffAntithetic << "\n";
        }

        // Close the opened file
        outfile.close();
    }
};



int main() {
    SpotVsOptionWriter spotVsOptionWriter;
    spotVsOptionWriter.writeData("../../analysis/SpotVsOption.csv");

    ConvergenceWriter convergenceWriter;
    convergenceWriter.writeData("../../analysis/Convergence.csv");

    OptionPriceVsVolatilityWriter optionPriceVsVolatilityWriter;
    optionPriceVsVolatilityWriter.writeData("../../analysis/OptionVsVolatility.csv");

    EfficiencyWriter efficiencyWriter;
    efficiencyWriter.writeData("../../analysis/Efficiency.csv");

    ToleranceWriter toleranceWriter;
    toleranceWriter.writeData("../../analysis/Tolerance.csv");

    return 0;
}
