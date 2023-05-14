#include <fstream>
#include <iomanip>
#include "../src/PricingEngine.hpp"
#include "../src/AsianOption.hpp"

int main() {
    // Open a file in write mode.
    std::ofstream outfile;
    outfile.open("../../analysis/AnalysisData.csv");

    // Write the headers
    outfile << "SpotPrice,OptionPrice\n";

    // Calculate prices for a range of spot prices
    for (double spot = 80.0; spot <= 120.0; spot += 1.0) {
        AsianOption option(105.0, 1.0, Option::Type::Call, AsianOption::AveragingType::Arithmetic, 10);
        double price = PricingEngine::calculatePriceNaive(option, spot, 0.05, 0.20, 10000);

        // Write the spot price and option price to the CSV file
        outfile << std::fixed << std::setprecision(2) << spot << "," << price << "\n";
    }

    // Close the opened file.
    outfile.close();

    return 0;
}
