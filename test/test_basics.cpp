
#include <cclipboard.h>
#include <gtest/gtest.h>
#include <stdio.h>

class BasicsTest : public ::testing::Test {
};

TEST_F(BasicsTest, TestBackwardsCompat) {
    cclipboard::CClipboard* clipboard = new cclipboard::CClipboard();

    ASSERT_TRUE(strcmp("test", clipboard->paste()) == 0);
}