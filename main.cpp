#include <iostream>
#include "Option.hpp"
#include "AsianOption.hpp"
#include "PricingEngine.hpp"

int main() {
    double spot_price = 100.0;
    double strike_price = 105.0;
    double risk_free_rate = 0.05;
    double volatility = 0.20;
    double expiry_time = 1.0;
    Option::Type optionType = Option::Type::Call;
    AsianOption::AveragingType averagingType = AsianOption::AveragingType::Arithmetic;
    unsigned int averagingPeriods = 12;

    AsianOption asianOption(strike_price, expiry_time, optionType, averagingType, averagingPeriods);
    PricingEngine pricingEngine;

    double optionPrice = pricingEngine.calculatePrice(asianOption, spot_price, risk_free_rate, volatility);
    std::cout << "Asian option price: " << optionPrice << std::endl;

    // You can add additional analysis methods here

    return 0;
}
