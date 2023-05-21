#include "Analysis.hpp"

// Specify what analysis you would like to see below by commenting out certain lines, then run the .ipynb file to generate graphs.
// NOTE: ONLY execute the required cells in the .ipynb file to avoid a FileNotFound Error.

int main() {
    SpotVsOptionWriter spotVsOptionWriter;
    SpotVsOptionWriter::writeData("../../analysis/SpotVsOption.csv");
/*
    ConvergenceWriter convergenceWriter;
    ConvergenceWriter::writeData("../../analysis/Convergence.csv");

    OptionPriceVsVolatilityWriter optionPriceVsVolatilityWriter;
    OptionPriceVsVolatilityWriter::writeData("../../analysis/OptionVsVolatility.csv");
*/
    EfficiencyWriter efficiencyWriter;
    EfficiencyWriter::writeData("../../analysis/Efficiency.csv");
/*
    ToleranceWriter toleranceWriter;
    ToleranceWriter::writeData("../../analysis/Tolerance.csv");
*/
    return 0;
}