#include <gtest/gtest.h>
#include "../src/Option.hpp"
#include "../src/AsianOption.hpp"

// Test fixture class for AsianOption testing
class AsianOptionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize objects for the test
        double strike_price = 105.0;
        double expiry_time = 1.0;
        AsianOption::AveragingType averagingType = AsianOption::AveragingType::Arithmetic;
        unsigned int averagingPeriods = 10;
        asianOption = new AsianOption(strike_price, expiry_time, Option::Type::Call, averagingType, averagingPeriods);
    }

    // Free any resources that were allocated for the test
    void TearDown() override {
        delete asianOption;
    }

    // Pointer to the AsianOption object for testing
    AsianOption* asianOption;
};

// Test case for getAveragingType() function
TEST_F(AsianOptionTest, GetAveragingType) {
    AsianOption::AveragingType expectedAveragingType = AsianOption::AveragingType::Arithmetic;
    EXPECT_EQ(expectedAveragingType, asianOption->getAveragingType());
}

// Test case for getAveragingPeriods() function
TEST_F(AsianOptionTest, GetAveragingPeriods) {
    unsigned int expectedAveragingPeriods = 10;
    EXPECT_EQ(expectedAveragingPeriods, asianOption->getAveragingPeriods());
}

// Test case for getStrike() function (inherited from Option class)
TEST_F(AsianOptionTest, GetStrike) {
    double expectedStrike = 105.0;
    EXPECT_EQ(expectedStrike, asianOption->getStrike());
}

// Test case for getExpiry() function (inherited from Option class)
TEST_F(AsianOptionTest, GetExpiry) {
    double expectedExpiry = 1.0;
    EXPECT_EQ(expectedExpiry, asianOption->getExpiry());
}

// Test case for getType() function (inherited from Option class)
TEST_F(AsianOptionTest, GetType) {
    Option::Type expectedType = Option::Type::Call;
    EXPECT_EQ(expectedType, asianOption->getType());
}

