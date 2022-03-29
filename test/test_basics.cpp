
#include <cclipboard.h>
#include <gtest/gtest.h>
#include <stdio.h>

class BasicsTest : public ::testing::Test {
};

TEST_F(BasicsTest, TestBackwardsCompat) {
    cclipboard::CClipboard* clipboard = new cclipboard::CClipboard();
    std::string text = "text";
    clipboard->copy(text);

    ASSERT_TRUE(strcmp(text.c_str(), clipboard->paste()) == 0);
}