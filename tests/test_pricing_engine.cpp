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

    PricingEngine* pricingEngine{};
    AsianOption* callOption{};
    AsianOption* putOption{};
    AsianOption* callOptionEdge{};
    AsianOption* callOptionG{};
    AsianOption* putOptionG{};
    AsianOption* callOptionEdgeG{};

    double spot_price{};
    double spot_price_edge{};
    double risk_free_rate{};
    double volatility{};
    unsigned int num_simulations{};
};

// Test case for ensuring all prices are greater than 0 for both averaging types
TEST_F(PricingEngineTest, NaiveCallGreaterZero) {
    double priceArithmetic = PricingEngine::calculatePriceNaive(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = PricingEngine::calculatePriceNaive(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, NaivePutGreaterZero) {
    double priceArithmetic = PricingEngine::calculatePriceNaive(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = PricingEngine::calculatePriceNaive(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, AntitheticCallGreaterZero) {
    double priceArithmetic = PricingEngine::calculatePriceAntithetic(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = PricingEngine::calculatePriceAntithetic(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, AntitheticPutGreaterZero) {
    double priceArithmetic = PricingEngine::calculatePriceAntithetic(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = PricingEngine::calculatePriceAntithetic(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, GBMCallGreaterZero) {
    double priceArithmetic = PricingEngine::calculatePriceGBM(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = PricingEngine::calculatePriceGBM(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

TEST_F(PricingEngineTest, GBMPutGreaterZero) {
    double priceArithmetic = PricingEngine::calculatePriceGBM(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double priceGeometric = PricingEngine::calculatePriceGBM(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_GT(priceArithmetic, 0);
    EXPECT_GT(priceGeometric, 0);
}

// Test case for ensuring that across methods (Naive, Antithetic, GBM), prices are close to their relevant option types (i.e. NaiveCall and AntitheticCall~GBMCall/
// NaivePut and AntitheticPut~GBMPut) for both averaging types. We use the GBM approximation as the "true price" - this has been justified in the report.
// Discussion: Discuss how tolerance is related to number of simulations performed. More simulations -> smaller tolerance since closer to true price
// Discussion: Discuss how tolerance of was chosen (to be linked to analysis)
TEST_F(PricingEngineTest, NaiveCallPriceNear) {
    double naivePriceArithmetic = PricingEngine::calculatePriceNaive(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double naivePriceGeometric = PricingEngine::calculatePriceNaive(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceArithmetic = PricingEngine::calculatePriceGBM(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceGeometric = PricingEngine::calculatePriceGBM(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(naivePriceGeometric, GBMPriceGeometric, 0.1);
}

TEST_F(PricingEngineTest, AntitheticCallPriceNear) {
    double antitheticPriceArithmetic = PricingEngine::calculatePriceAntithetic(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceGeometric = PricingEngine::calculatePriceAntithetic(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceArithmetic = PricingEngine::calculatePriceGBM(*callOption, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceGeometric = PricingEngine::calculatePriceGBM(*callOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(antitheticPriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(antitheticPriceGeometric, GBMPriceGeometric, 0.1);
}

TEST_F(PricingEngineTest, NaivePutPriceNear) {
    double naivePriceArithmetic = PricingEngine::calculatePriceNaive(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double naivePriceGeometric = PricingEngine::calculatePriceNaive(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceArithmetic = PricingEngine::calculatePriceGBM(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceGeometric = PricingEngine::calculatePriceGBM(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(naivePriceGeometric, GBMPriceGeometric, 0.1);
}

TEST_F(PricingEngineTest, AntitheticPutPriceNear) {
    double antitheticPriceArithmetic = PricingEngine::calculatePriceAntithetic(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double antitheticPriceGeometric = PricingEngine::calculatePriceAntithetic(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceArithmetic = PricingEngine::calculatePriceGBM(*putOption, spot_price, risk_free_rate, volatility, num_simulations);
    double GBMPriceGeometric = PricingEngine::calculatePriceGBM(*putOptionG, spot_price, risk_free_rate, volatility, num_simulations);
    EXPECT_NEAR(antitheticPriceArithmetic, GBMPriceArithmetic, 0.1);
    EXPECT_NEAR(antitheticPriceGeometric, GBMPriceGeometric, 0.1);
}

// Edge test case ensuring all three methods return a price of zero for call options when spot_price=0.1 across both averaging types
TEST_F(PricingEngineTest, NaiveCallEdgeCaseSpot) {
    double naivePriceArithmetic = PricingEngine::calculatePriceNaive(*callOption, spot_price_edge, risk_free_rate, volatility,num_simulations);
    double naivePriceGeometric = PricingEngine::calculatePriceNaive(*callOptionG, spot_price_edge, risk_free_rate, volatility,num_simulations);
    EXPECT_DOUBLE_EQ(naivePriceArithmetic, 0.0);
    EXPECT_DOUBLE_EQ(naivePriceGeometric, 0.0);
}

TEST_F(PricingEngineTest, AntitheticCallEdgeCaseSpot) {
    double antitheticPriceArithmetic = PricingEngine::calculatePriceAntithetic(*callOption, spot_price_edge, risk_free_rate, volatility,num_simulations);
    double antitheticPriceGeometric = PricingEngine::calculatePriceAntithetic(*callOptionG, spot_price_edge, risk_free_rate, volatility,num_simulations);
    EXPECT_DOUBLE_EQ(antitheticPriceArithmetic, 0.0);
    EXPECT_DOUBLE_EQ(antitheticPriceGeometric, 0.0);
}

TEST_F(PricingEngineTest, GBMCallEdgeCaseSpot) {
    double GBMPriceArithmetic = PricingEngine::calculatePriceGBM(*callOption, spot_price_edge, risk_free_rate, volatility,num_simulations);
    double GBMPriceGeometric = PricingEngine::calculatePriceGBM(*callOptionG, spot_price_edge, risk_free_rate, volatility,num_simulations);
    EXPECT_DOUBLE_EQ(GBMPriceArithmetic, 0.0);
    EXPECT_DOUBLE_EQ(GBMPriceGeometric, 0.0);
}

// Edge test case ensuring both methods return a price of zero for call options when expiry time is imminent (0.01) across both averaging types (to 5dp)
TEST_F(PricingEngineTest, NaiveCallEdgeCaseExpiry) {
    double naivePriceArithmetic = PricingEngine::calculatePriceNaive(*callOptionEdge, spot_price, risk_free_rate, volatility,num_simulations);
    double naivePriceGeometric = PricingEngine::calculatePriceNaive(*callOptionEdgeG, spot_price, risk_free_rate, volatility,num_simulations);
    EXPECT_NEAR(naivePriceArithmetic, 0.0, 0.00001);
    EXPECT_NEAR(naivePriceGeometric, 0.0, 0.00001);
}

TEST_F(PricingEngineTest, AntitheticCallEdgeCaseExpiry) {
    double antitheticPriceArithmetic = PricingEngine::calculatePriceAntithetic(*callOptionEdge, spot_price, risk_free_rate, volatility,num_simulations);
    double antitheticPriceGeometric = PricingEngine::calculatePriceAntithetic(*callOptionEdgeG, spot_price, risk_free_rate, volatility,num_simulations);
    EXPECT_NEAR(antitheticPriceArithmetic, 0.0, 0.00001);
    EXPECT_NEAR(antitheticPriceGeometric, 0.0, 0.00001);
}

TEST_F(PricingEngineTest, GBMCallEdgeCaseExpiry) {
    double GBMPriceArithmetic = PricingEngine::calculatePriceGBM(*callOptionEdge, spot_price, risk_free_rate, volatility,num_simulations);
    double GBMPriceGeometric = PricingEngine::calculatePriceGBM(*callOptionEdgeG, spot_price, risk_free_rate, volatility,num_simulations);
    EXPECT_NEAR(GBMPriceArithmetic, 0.0, 0.00001);
    EXPECT_NEAR(GBMPriceGeometric, 0.0, 0.00001);
}