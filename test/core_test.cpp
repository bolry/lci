/*
 * Lint compiler interceptor
 * Copyright (C) 2013 Bo Rydberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "inc/core.h"

#include <gmock/gmock.h>

using namespace testing;

namespace {

char const empty[0+1] = "";
char const another_empty[0+1] = "";
char const dash[] = "-";
char const another_dash[] = "-";
char const str_one[1+1] = "a";
char const str_two[2+1] = "bc";

TEST(ParseBoolFlagWithEmpty, TestDataPrecondition)
{
	ASSERT_THAT(empty, Ne(another_empty));
	ASSERT_THAT(empty, StrEq(another_empty));
}

TEST(ParseBoolFlagWithEmpty, EmptyExact)
{
	EXPECT_TRUE(parse_bool_flag(empty, another_empty, -1));
	EXPECT_TRUE(parse_bool_flag(empty, empty, -1));
}
TEST(ParseBoolFlagWithEmpty, EmptyToSomethingExact)
{
	EXPECT_FALSE(parse_bool_flag(empty, str_one, -1));
	EXPECT_FALSE(parse_bool_flag(str_one, empty, -1));
	EXPECT_FALSE(parse_bool_flag(empty, str_two, -1));
	EXPECT_FALSE(parse_bool_flag(str_two, empty, -1));
}
TEST(ParseBoolFlagWithEmpty, EmptyZero)
{
	EXPECT_TRUE(parse_bool_flag(empty, another_empty, 0));
	EXPECT_TRUE(parse_bool_flag(empty, empty, 0));
}
TEST(ParseBoolFlagWithEmpty, EmptyToSomethingZero)
{
	EXPECT_TRUE(parse_bool_flag(empty, str_one, 0));
	EXPECT_FALSE(parse_bool_flag(str_one, empty, 0));
	EXPECT_TRUE(parse_bool_flag(empty, str_two, 0));
	EXPECT_FALSE(parse_bool_flag(str_two, empty, 0));
}
TEST(ParseBoolFlagWithEmpty, EmptyToFirstChar)
{
	EXPECT_FALSE(parse_bool_flag(empty, another_empty, 1));
	EXPECT_FALSE(parse_bool_flag(empty, empty, 1));
}
TEST(ParseBoolFlagWithEmpty, EmptyToSomethingToFirstChar)
{
	EXPECT_FALSE(parse_bool_flag(empty, str_one, 1));
	EXPECT_FALSE(parse_bool_flag(str_one, empty, 1));
	EXPECT_FALSE(parse_bool_flag(empty, str_two, 1));
	EXPECT_FALSE(parse_bool_flag(str_two, empty, 1));
}

TEST(ParseBoolFlagWithDash, TestDataPrecondition)
{
	ASSERT_THAT(dash, Ne(another_dash));
	ASSERT_THAT(dash, StrEq(another_dash));
}

TEST(ParseBoolFlagWithDash, Exact)
{
	EXPECT_TRUE(parse_bool_flag(dash, another_dash, -1));
	EXPECT_TRUE(parse_bool_flag(dash, dash, -1));
}
TEST(ParseBoolFlagWithDash, DashoSomethingExact)
{
	EXPECT_FALSE(parse_bool_flag(dash, str_one, -1));
	EXPECT_FALSE(parse_bool_flag(str_one, dash, -1));
	EXPECT_FALSE(parse_bool_flag(dash, str_two, -1));
	EXPECT_FALSE(parse_bool_flag(str_two, dash, -1));
}
TEST(ParseBoolFlagWithDash, DashZero)
{
	EXPECT_TRUE(parse_bool_flag(dash, another_dash, 0));
	EXPECT_TRUE(parse_bool_flag(dash, dash, 0));
}
TEST(ParseBoolFlagWithDash, DashToSomethingZero)
{
	EXPECT_FALSE(parse_bool_flag(dash, str_one, 0));
	EXPECT_FALSE(parse_bool_flag(str_one, dash, 0));
	EXPECT_FALSE(parse_bool_flag(dash, str_two, 0));
	EXPECT_FALSE(parse_bool_flag(str_two, dash, 0));
}
TEST(ParseBoolFlagWithDash, DashToFirstChar)
{
	EXPECT_TRUE(parse_bool_flag(dash, another_dash, 1));
	EXPECT_TRUE(parse_bool_flag(dash, dash, 1));
}
TEST(ParseBoolFlagWithDash, DashToSomethingToFirstChar)
{
	EXPECT_FALSE(parse_bool_flag(dash, str_one, 1));
	EXPECT_FALSE(parse_bool_flag(str_one, dash, 1));
	EXPECT_FALSE(parse_bool_flag(dash, str_two, 1));
	EXPECT_FALSE(parse_bool_flag(str_two, dash, 1));
}
/*
 *
 */
TEST(ParseBoolFlagWithDash, DashToDashieExact)
{
	EXPECT_FALSE(parse_bool_flag(dash, "-a", -1));
	EXPECT_FALSE(parse_bool_flag("-a", dash, -1));
}
TEST(ParseBoolFlagWithDash, DashToDashieFirstChar)
{
	EXPECT_TRUE(parse_bool_flag(dash, "-a", 1));
	EXPECT_FALSE(parse_bool_flag("-a", dash, 1));
}

} // namespace
