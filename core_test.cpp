#include "core.h"

#include <gmock/gmock.h>

namespace {

using namespace testing;

char const empty[0+1] = "";
char const another_empty[0+1] = "";
char const dash[] = "-";
char const another_dash[] = "-";
char const str_one[1+1] = "a";
char const str_two[2+1] = "bc";

TEST(ParseBoolFlagWithEmptyTest, ConstantsAreOK)
{
	ASSERT_THAT(empty, Ne(another_empty));
	ASSERT_THAT(empty, StrEq(another_empty));
}

TEST(ParseBoolFlagWithEmptyTest, EmptyExact)
{
	EXPECT_TRUE(parse_bool_flag(empty, another_empty, -1));
	EXPECT_TRUE(parse_bool_flag(empty, empty, -1));
}
TEST(ParseBoolFlagWithEmptyTest, EmptyToSomethingExact)
{
	EXPECT_FALSE(parse_bool_flag(empty, str_one, -1));
	EXPECT_FALSE(parse_bool_flag(str_one, empty, -1));
	EXPECT_FALSE(parse_bool_flag(empty, str_two, -1));
	EXPECT_FALSE(parse_bool_flag(str_two, empty, -1));
}
TEST(ParseBoolFlagWithEmptyTest, EmptyZero)
{
	EXPECT_TRUE(parse_bool_flag(empty, another_empty, 0));
	EXPECT_TRUE(parse_bool_flag(empty, empty, 0));
}
TEST(ParseBoolFlagWithEmptyTest, EmptyToSomethingZero)
{
	EXPECT_TRUE(parse_bool_flag(empty, str_one, 0));
	EXPECT_FALSE(parse_bool_flag(str_one, empty, 0));
	EXPECT_TRUE(parse_bool_flag(empty, str_two, 0));
	EXPECT_FALSE(parse_bool_flag(str_two, empty, 0));
}
TEST(ParseBoolFlagWithEmptyTest, EmptyToFirstChar)
{
	EXPECT_FALSE(parse_bool_flag(empty, another_empty, 1));
	EXPECT_FALSE(parse_bool_flag(empty, empty, 1));
}
TEST(ParseBoolFlagWithEmptyTest, EmptyToSomethingToFirstChar)
{
	EXPECT_FALSE(parse_bool_flag(empty, str_one, 1));
	EXPECT_FALSE(parse_bool_flag(str_one, empty, 1));
	EXPECT_FALSE(parse_bool_flag(empty, str_two, 1));
	EXPECT_FALSE(parse_bool_flag(str_two, empty, 1));
}
/*
 *
 */
TEST(ParseBoolFlagWithDashTest, ConstantsAreOK)
{
	ASSERT_THAT(dash, Ne(another_dash));
	ASSERT_THAT(dash, StrEq(another_dash));
}

TEST(ParseBoolFlagWithDashTest, Exact)
{
	EXPECT_NE(dash, another_dash);
	EXPECT_TRUE(parse_bool_flag(dash, another_dash, -1));
	EXPECT_TRUE(parse_bool_flag(dash, dash, -1));
}
TEST(ParseBoolFlagWithDashTest, DashoSomethingExact)
{
	EXPECT_FALSE(parse_bool_flag(dash, str_one, -1));
	EXPECT_FALSE(parse_bool_flag(str_one, dash, -1));
	EXPECT_FALSE(parse_bool_flag(dash, str_two, -1));
	EXPECT_FALSE(parse_bool_flag(str_two, dash, -1));
}
TEST(ParseBoolFlagWithDashTest, DashZero)
{
	EXPECT_NE(dash, another_dash);
	EXPECT_TRUE(parse_bool_flag(dash, another_dash, 0));
	EXPECT_TRUE(parse_bool_flag(dash, dash, 0));
}
TEST(ParseBoolFlagWithDashTest, DashToSomethingZero)
{
	EXPECT_FALSE(parse_bool_flag(dash, str_one, 0));
	EXPECT_FALSE(parse_bool_flag(str_one, dash, 0));
	EXPECT_FALSE(parse_bool_flag(dash, str_two, 0));
	EXPECT_FALSE(parse_bool_flag(str_two, dash, 0));
}
TEST(ParseBoolFlagWithDashTest, DashToFirstChar)
{
	EXPECT_NE(dash, another_dash);
	EXPECT_TRUE(parse_bool_flag(dash, another_dash, 1));
	EXPECT_TRUE(parse_bool_flag(dash, dash, 1));
}
TEST(ParseBoolFlagWithDashTest, DashToSomethingToFirstChar)
{
	EXPECT_FALSE(parse_bool_flag(dash, str_one, 1));
	EXPECT_FALSE(parse_bool_flag(str_one, dash, 1));
	EXPECT_FALSE(parse_bool_flag(dash, str_two, 1));
	EXPECT_FALSE(parse_bool_flag(str_two, dash, 1));
}
/*
 *
 */
TEST(ParseBoolFlagWithDashTest, DashToDashieExact)
{
	EXPECT_FALSE(parse_bool_flag(dash, "-a", -1));
	EXPECT_FALSE(parse_bool_flag("-a", dash, -1));
}
TEST(ParseBoolFlagWithDashTest, DashToDashieFirstChar)
{
	EXPECT_TRUE(parse_bool_flag(dash, "-a", 1));
	EXPECT_FALSE(parse_bool_flag("-a", dash, 1));
}

} // namespace
