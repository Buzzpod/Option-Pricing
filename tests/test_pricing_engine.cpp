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
        double expiry_time_edge = 0.01;
        unsigned int averagingPeriods = 10;

        // Initialising option with arithmetic averaging type
        AsianOption::AveragingType averagingTypeArithmetic = AsianOption::AveragingType::Arithmetic;
        callOption = new AsianOption(strike_price, expiry_time, Option::Type::Call, averagingTypeArithmetic, averagingPeriods);
        putOption = new AsianOption(strike_price, expiry_time, Option::Type::Put, averagingTypeArithmetic, averagingPeriods);
        callOptionEdge = new AsianOption(strike_price, expiry_time_edge, Option::Type::Call, averagingTypeArithmetic, averagingPeriods);

        // Initialising option with geometric averaging type
        AsianOption::AveragingType averagingTypeGeometric = AsianOption::AveragingType::Geometric;
        callOptionG = new AsianOption(strike_price, expiry_time, Option::Type::Call, averagingTypeGeometric, averagingPeriods);
        putOptionG = new AsianOption(strike_price, expiry_time, Option::Type::Put, averagingTypeGeometric, averagingPeriods);
        callOptionEdgeG = new AsianOption(strike_price, expiry_time_edge, Option::Type::Call, averagingTypeGeometric, averagingPeriods);

        spot_price = 100.0;
        risk_free_rate = 0.05;
        volatility = 0.20;
        num_simulations = 100000;
        spot_price_edge = 0.1;
    }

    // Free any resources that were allocated for the test
    void TearDown() override {
        delete pricingEngine;
        delete callOption;
        delete putOption;
        delete callOptionEdge;
        delete callOptionG;
        delete putOptionG;
        delete callOptionEdgeG;
    }

    PricingEngine* pricingEngine;
    AsianOption* callOption;
    AsianOption* putOption;
    AsianOption* callOptionEdge;
    AsianOption* callOptionG;
    AsianOption* putOptionG;
    AsianOption* callOptionEdgeG;

    double spot_price;
    double spot_price_edge;
    double risk_free_rate;
    double volatility;
    unsigned int num_simulations;
};

// Begin by ensuring all prices are greater than 0 for both averaging types
TEST_F(PricingEngineTest, TestCalculatePriceNaiveCallZero) {
    double priceArithmetic = pricingEngine->calculatePriceNaive(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->calculatePriceNaive(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceNaivePutZero) {
    double priceArithmetic = pricingEngine->calculatePriceNaive(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->calculatePriceNaive(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticCallZero) {
    double priceArithmetic = pricingEngine->calculatePriceAntithetic(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->calculatePriceAntithetic(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticPutZero) {
    double priceArithmetic = pricingEngine->calculatePriceAntithetic(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->calculatePriceAntithetic(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

// Across methods (Naive and Antithetic), we expect prices to be close for their relevant options types (i.e. NaiveCall~AntitheticCall and NaivePut~AntitheticPut)
// for both averaging types
TEST_F(PricingEngineTest, TestCallPriceNear) {
    double naivePriceArithmetic = pricingEngine->calculatePriceNaive(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceArithmetic = pricingEngine->calculatePriceAntithetic(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double naivePriceGeometric = pricingEngine->calculatePriceNaive(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceGeometric = pricingEngine->calculatePriceAntithetic(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, antitheticPriceArithmetic, 0.1);
    EXPECT_NEAR(naivePriceGeometric, antitheticPriceGeometric, 0.1);
}

TEST_F(PricingEngineTest, TestPutPriceNear) {
    double naivePriceArithmetic = pricingEngine->calculatePriceNaive(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceArithmetic = pricingEngine->calculatePriceAntithetic(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double naivePriceGeometric = pricingEngine->calculatePriceNaive(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceGeometric = pricingEngine->calculatePriceAntithetic(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, antitheticPriceArithmetic, 0.1);
    EXPECT_NEAR(naivePriceGeometric, antitheticPriceGeometric, 0.1);
}

// Using an online calculator (INSERT LINK), ensure all prices are near the calculated price, by specifying a suitable tolerance level
// Extension idea: Use expected volatility for tolerance or look at other methods for calculating tolerance
// Extension: Find an API to call to calculate prices and take an average, then specify tolerance level
// Discussion: Discuss how tolerance is related to number of simulations performed. More simulations -> smaller tolerance since closer to true price
TEST_F(PricingEngineTest, TestCalculatePriceNaiveCallNear) {
    double price = pricingEngine->calculatePriceNaive(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(price, 3.126, 0.2);
}

TEST_F(PricingEngineTest, TestCalculatePriceNaivePutNear) {
    double price = pricingEngine->calculatePriceNaive(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(price, 5.743, 0.2);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticCallNear) {
    double price = pricingEngine->calculatePriceAntithetic(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(price, 3.126, 0.2);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticPutNear) {
    double price = pricingEngine->calculatePriceAntithetic(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(price, 5.743, 0.2);
}

// (Edge case): Ensure both methods return a price of zero for call options when spot_price=0.1 across both averaging types
TEST_F(PricingEngineTest, TestCalculatePriceNaiveCallEdgeSpot) {
    double naivePriceArithmetic = pricingEngine->calculatePriceNaive(*callOption, spot_price_edge, risk_free_rate, volatility,num_simulations);
    double naivePriceGeometric = pricingEngine->calculatePriceNaive(*callOptionG, spot_price_edge, risk_free_rate, volatility,num_simulations);
    EXPECT_DOUBLE_EQ(naivePriceArithmetic, 0.0);
    EXPECT_DOUBLE_EQ(naivePriceGeometric, 0.0);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticCallEdgeSpot) {
    double antitheticPriceArithmetic = pricingEngine->calculatePriceAntithetic(*callOption, spot_price_edge, risk_free_rate, volatility,num_simulations);
    double antitheticPriceGeometric = pricingEngine->calculatePriceAntithetic(*callOptionG, spot_price_edge, risk_free_rate, volatility,num_simulations);
    EXPECT_DOUBLE_EQ(antitheticPriceArithmetic, 0.0);
    EXPECT_DOUBLE_EQ(antitheticPriceGeometric, 0.0);
}

// (Edge case): Ensure both methods return a price of zero for call options when expiry time is imminent (0.01) across both averaging types (to 5dp)
TEST_F(PricingEngineTest, TestCalculatePriceNaiveCallEdgeExpiry) {
    double naivePriceArithmetic = pricingEngine->calculatePriceNaive(*callOptionEdge, spot_price, risk_free_rate, volatility,num_simulations);
    double naivePriceGeometric = pricingEngine->calculatePriceNaive(*callOptionEdgeG, spot_price, risk_free_rate, volatility,num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, 0.0, 0.00001);
    EXPECT_NEAR(naivePriceGeometric, 0.0, 0.00001);
}

TEST_F(PricingEngineTest, TestCalculatePriceAntitheticCallEdgeExpiry) {
    double antitheticPriceArithmetic = pricingEngine->calculatePriceAntithetic(*callOptionEdge, spot_price, risk_free_rate, volatility,num_simulations);
    double antitheticPriceGeometric = pricingEngine->calculatePriceAntithetic(*callOptionEdgeG, spot_price, risk_free_rate, volatility,num_simulations);
    EXPECT_NEAR(antitheticPriceArithmetic, 0.0, 0.00001);
    EXPECT_NEAR(antitheticPriceGeometric, 0.0, 0.00001);
}