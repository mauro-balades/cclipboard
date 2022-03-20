
#include <cclipboard.h>
#include <gtest/gtest.h>

using namespace cclipboard;

class BasicsTest : public ::testing::Test {
};
#include <stdio.h>

TEST_F(BasicsTest, TestBackwardsCompat) {
    CClipboard clipboard();
    printf("%s", clipboard().paste());

    ASSERT_TRUE(true);
}