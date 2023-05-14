#include "gtest/gtest.h"
#include "../src/PricingEngine.hpp"
#include "../src/AsianOption.hpp"

class PricingEngineTest : public ::testing::Test {
protected:
    // Initialize objects for the test
    void SetUp() override {
        pricingEngine = new PricingEngine();
        double strike_price = 105.0;
        double expiry_time = 1.0;
        AsianOption::AveragingType averagingType = AsianOption::AveragingType::Arithmetic;
        unsigned int averagingPeriods = 10;
        callOption = new AsianOption(strike_price, expiry_time, Option::Type::Call, averagingType, averagingPeriods);
        putOption = new AsianOption(strike_price, expiry_time, Option::Type::Put, averagingType, averagingPeriods);
        spot_price = 100.0;
        risk_free_rate = 0.05;
        volatility = 0.20;
        num_simulations = 100000;
    }

    // Free any resources that were allocated for the test
    void TearDown() override {
        delete pricingEngine;
        delete callOption;
        delete putOption;
    }

    PricingEngine* pricingEngine;
    AsianOption* callOption;
    AsianOption* putOption;
    double spot_price;
    double risk_free_rate;
    double volatility;
    unsigned int num_simulations;
};

TEST_F(PricingEngineTest, TestCalculatePriceNaiveCall) {
    double price = pricingEngine->calculatePriceNaive(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(price, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceNaivePut) {
    double price = pricingEngine->calculatePriceNaive(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(price, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticCall) {
    double price = pricingEngine->calculatePriceAntithetic(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(price, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticPut) {
    double price = pricingEngine->calculatePriceAntithetic(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(price, 0);
}
