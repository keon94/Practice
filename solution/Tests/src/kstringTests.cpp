#include <gtest/gtest.h>
#include "kstring.h"

class KstringTestsFixture : public testing::Test {

};

TEST(KstringTests, implicitConstructorTest) {
	Kstring t = "a1";
	Kstring u = "a2";
	Kstring v = "abcdefg hij";
	ASSERT_STREQ(t.to_Cstring(), "a1\0");
	ASSERT_STREQ(u.to_Cstring(), "a2\0");
	ASSERT_STREQ(v.to_Cstring(), "abcdefg hij\0");
}



TEST(KstringTests, concactenationTest) {



}