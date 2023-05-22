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

// Test case for ensuring all prices are greater than 0 for both averaging types
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

TEST_F(PricingEngineTest, TestCalculatePriceGBMCallZero) {
    double priceArithmetic = pricingEngine->calculatePriceGBM(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->calculatePriceGBM(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceControlVariatePutZero) {
    double priceArithmetic = pricingEngine->calculatePriceGBM(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->calculatePriceGBM(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceControlVariateCallZero) {
    double priceArithmetic = pricingEngine->SDE_control_variate_2(*callOption, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->SDE_control_variate_2(*callOptionG, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, TestCalculatePriceControlVariateCallZero) {
    double priceArithmetic = pricingEngine->SDE_control_variate_2(*callOption, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = pricingEngine->SDE_control_variate_2(*callOptionG, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

// Test case for ensuring that across methods (Naive, Antithetic, GBM), prices are close to their relevant option types (i.e. NaiveCall and AntitheticCall~GBMCall/
// NaivePut and AntitheticPut~GBMPut) for both averaging types. We use the GBM approximation as the "true price" - this has been justified in the report.
// Discussion: Discuss how tolerance is related to number of simulations performed. More simulations -> smaller tolerance since closer to true price
// Discussion: Discuss how tolerance of was chosen (to be linked to analysis)
TEST_F(PricingEngineTest, TestCallPriceNear) {
    double naivePriceArithmetic = pricingEngine->calculatePriceNaive(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceArithmetic = pricingEngine->calculatePriceAntithetic(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double controlvariatePriceArithmetic = pricingEngine->SDE_control_variate_2(*callOption, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceArithmetic = pricingEngine->calculatePriceGBM(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double naivePriceGeometric = pricingEngine->calculatePriceNaive(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceGeometric = pricingEngine->calculatePriceAntithetic(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double controlvariatePriceGeometric = pricingEngine->SDE_control_variate_2(*callOptionG, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceGeometric = pricingEngine->calculatePriceGBM(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(antitheticPriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(controlvariatePriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(naivePriceGeometric, GBMPriceGeometric, 0.1);
    EXPECT_NEAR(controlvariatePriceGeometric, GBMPriceGeometric, 0.1);
    EXPECT_NEAR(antitheticPriceArithmetic, GBMPriceArithmetic, 0.1);
}

TEST_F(PricingEngineTest, TestPutPriceNear) {
    double naivePriceArithmetic = pricingEngine->calculatePriceNaive(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceArithmetic = pricingEngine->calculatePriceAntithetic(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double controlvariatePriceArithmetic = pricingEngine->SDE_control_variate_2(*putOption, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceArithmetic = pricingEngine->calculatePriceGBM(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double naivePriceGeometric = pricingEngine->calculatePriceNaive(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceGeometric = pricingEngine->calculatePriceAntithetic(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double controlvariatePriceGeometric = pricingEngine->SDE_control_variate_2(*putOptionG, spot_price, strike_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceGeometric = pricingEngine->calculatePriceGBM(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(antitheticPriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(controlvariatePriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(naivePriceGeometric, GBMPriceGeometric, 0.1);
    EXPECT_NEAR(antitheticPriceGeometric, GBMPriceGeometric, 0.1);
    EXPECT_NEAR(controlvariatePriceGeometric, GBMPriceGeometric, 0.1);
}

// Edge test case ensuring all three methods return a price of zero for call options when spot_price=0.1 across both averaging types
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

TEST_F(PricingEngineTest, TestCalculatePriceGBMCallEdgeSpot) {
    double GBMPriceArithmetic = pricingEngine->calculatePriceGBM(*callOption, spot_price_edge, risk_free_rate, volatility,num_simulations);
    double GBMPriceGeometric = pricingEngine->calculatePriceGBM(*callOptionG, spot_price_edge, risk_free_rate, volatility,num_simulations);
    EXPECT_DOUBLE_EQ(GBMPriceArithmetic, 0.0);
    EXPECT_DOUBLE_EQ(GBMPriceGeometric, 0.0);
}

TEST_F(PricingEngineTest, TestCalculatePriceControlVariateCallEdgeSpot) {
    double controlvariatePriceArithmetic = pricingEngine->SDE_control_variate_2(*callOption, spot_price_edge, strike_price, risk_free_rate, volatility,num_simulations);
    double controlvariatePriceGeometric = pricingEngine->SDE_control_variate_2(*callOptionG, spot_price_edge, strike_price, risk_free_rate, volatility,num_simulations);
    EXPECT_DOUBLE_EQ(controlvariatePriceArithmetic, 0.0);
    EXPECT_DOUBLE_EQ(controlvariatePriceGeometric, 0.0);
}

// Edge test case ensuring both methods return a price of zero for call options when expiry time is imminent (0.01) across both averaging types (to 5dp)
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

TEST_F(PricingEngineTest, TestCalculatePriceGBMCallEdgeExpiry) {
    double GBMPriceArithmetic = pricingEngine->calculatePriceGBM(*callOptionEdge, spot_price, risk_free_rate, volatility,num_simulations);
    double GBMPriceGeometric = pricingEngine->calculatePriceGBM(*callOptionEdgeG, spot_price, risk_free_rate, volatility,num_simulations);
    EXPECT_NEAR(GBMPriceArithmetic, 0.0, 0.00001);
    EXPECT_NEAR(GBMPriceGeometric, 0.0, 0.00001);
}

TEST_F(PricingEngineTest, TestCalculatePriceControlVariateCallEdgeExpiry) {
    double controlvariatePriceArithmetic = pricingEngine->SDE_control_variate_2(*callOptionEdge, spot_price, strike_price, risk_free_rate, volatility,num_simulations);
    double controlvariatePriceGeometric = pricingEngine->SDE_control_variate_2(*callOptionEdgeG, spot_price, strike_price, risk_free_rate, volatility,num_simulations);
    EXPECT_NEAR(controlvariatePriceArithmetic, 0.0, 0.00001);
    EXPECT_NEAR(controlvariatePriceGeometric, 0.0, 0.00001);
}
