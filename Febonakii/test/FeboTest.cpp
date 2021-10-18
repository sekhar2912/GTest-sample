#include "gtest/gtest.h"
#include "Analyzer.hpp"
#include "TestHelper.hpp" // hidden file

class FeboTest : public ::testing::Test {
protected:
     
    Analyzer analyzer;
    virtual void TearDown() { }
};


TEST_F(FeboTest, ValueForTest) {
    bool result = testResults(analyzer, 9);
    EXPECT_TRUE(result);
}
