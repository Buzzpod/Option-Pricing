#include "Analysis.hpp"

// Specify what analysis you would like to see below by commenting out certain lines, then run the .ipynb file to generate graphs.
// NOTE: ONLY execute the required cells in the .ipynb file to avoid a FileNotFound Error.
// File takes 5 minutes to run all analysis.

int main() {
    SpotVsOptionWriter::writeData("../../analysis/SpotVsOption.csv", Option::Type::Call, AsianOption::AveragingType::Arithmetic, PricingEngine::calculatePriceGBM);

    ConvergenceWriter convergenceWriter;
    ConvergenceWriter::writeData("../../analysis/Convergence.csv", Option::Type::Call, AsianOption::AveragingType::Arithmetic, PricingEngine::calculatePriceGBM);

    OptionPriceVsVolatilityWriter::writeData("../../analysis/OptionVsVolatility.csv", Option::Type::Call, AsianOption::AveragingType::Arithmetic, PricingEngine::calculatePriceGBM);

    EfficiencyWriter efficiencyWriter;
    EfficiencyWriter::writeData("../../analysis/Efficiency.csv");

    ToleranceWriter toleranceWriter;
    ToleranceWriter::writeData("../../analysis/Tolerance.csv", Option::Type::Call, AsianOption::AveragingType::Arithmetic, PricingEngine::calculatePriceGBM);

    return 0;
}