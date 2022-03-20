
#include <cclipboard.h>
#include <gtest/gtest.h>

using namespace cclipboard;

class BasicsTest : public ::testing::Test {
};

TEST_F(BasicsTest, TestBackwardsCompat) {
    CClipboard clipboard();

    
    ASSERT_TRUE(true);
}