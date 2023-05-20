#include <gtest/gtest.h>
#include "Option.hpp"

// Derived class implementing Option
class TestOption : public Option {
public:
    TestOption(double strike, double expiry, Type type)
            : Option(strike, expiry, type) {}

    double payoff(double underlyingPrice) const override {
        // Implement the payoff calculation based on the underlying price
        if (type == Type::Call) {
            return std::max(underlyingPrice - strike, 0.0);
        } else {
            return std::max(strike - underlyingPrice, 0.0);
        }
    }
};

// Test fixture class for Option testing
class OptionTest : public ::testing::Test {
protected:
    // Initialize objects for the test
    void SetUp() override {
        double strike_price = 105.0;
        double expiry_time = 1.0;
        option = new TestOption(strike_price, expiry_time, Option::Type::Call);
    }

    // Free any resources that were allocated for the test
    void TearDown() override {
        delete option;
    }

    // Pointer to the Option object for testing
    Option* option;
};

// Test case for getStrike() function
TEST_F(OptionTest, TestGetStrike) {
    double expectedStrike = 105.0;
    EXPECT_EQ(expectedStrike, option->getStrike());
}

// Test case for getExpiry() function
TEST_F(OptionTest, TestGetExpiry) {
    double expectedExpiry = 1.0;
    EXPECT_EQ(expectedExpiry, option->getExpiry());
}

// Test case for getType() function
TEST_F(OptionTest, TestGetType) {
    Option::Type expectedType = Option::Type::Call;
    EXPECT_EQ(expectedType, option->getType());
}
