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

char const empty[0 + 1] = "";
char const another_empty[0 + 1] = "";
char const dash[] = "-";
char const another_dash[] = "-";
char const str_one[1 + 1] = "a";
char const str_two[2 + 1] = "bc";

TEST(ParseBoolFlag, TestdataPreconditions)
{
	ASSERT_THAT(empty, Ne(another_empty));
	ASSERT_THAT(empty, StrEq(another_empty));
}

TEST(ParseBoolFlag,StrstrSearchOnEmptyNeedle)
{
	char const emptyHaystack[1] = "";
	char const emptyNeedle[1] = "";
	char const NonEmptyHaystack[] = "moo";

	ASSERT_THAT(strstr(emptyHaystack, emptyNeedle), Eq(emptyHaystack));
	ASSERT_THAT(strstr(emptyNeedle, emptyNeedle), Eq(emptyNeedle));
	ASSERT_THAT(strstr(NonEmptyHaystack, emptyNeedle),
			Eq(NonEmptyHaystack));
}

TEST(ParseBoolFlag, EmptyExact)
{
	ASSERT_TRUE(parse_bool_flag(empty, another_empty, -1));
	ASSERT_TRUE(parse_bool_flag(empty, empty, -1));
}
TEST(ParseBoolFlag, EmptyToSomethingExact)
{
	ASSERT_FALSE(parse_bool_flag(empty, str_one, -1));
	ASSERT_FALSE(parse_bool_flag(str_one, empty, -1));
}
TEST(ParseBoolFlag, EmptyZero)
{
	ASSERT_TRUE(parse_bool_flag(empty, another_empty, 0));
	ASSERT_TRUE(parse_bool_flag(empty, empty, 0));
}
TEST(ParseBoolFlag, EmptyToSomethingZero)
{
	ASSERT_TRUE(parse_bool_flag(empty, str_one, 0));
	ASSERT_FALSE(parse_bool_flag(str_one, empty, 0));
}
TEST(ParseBoolFlag, EmptyToFirstChar)
{
	ASSERT_FALSE(parse_bool_flag(empty, another_empty, 1));
	ASSERT_FALSE(parse_bool_flag(empty, empty, 1));
}
TEST(ParseBoolFlag, EmptyToSomethingToFirstChar)
{
	ASSERT_FALSE(parse_bool_flag(empty, str_one, 1));
	ASSERT_FALSE(parse_bool_flag(str_one, empty, 1));
	ASSERT_FALSE(parse_bool_flag(empty, str_two, 1));
	ASSERT_FALSE(parse_bool_flag(str_two, empty, 1));
}

TEST(ParseBoolFlag, Exact)
{
	ASSERT_TRUE(parse_bool_flag(dash, another_dash, -1));
	ASSERT_TRUE(parse_bool_flag(dash, dash, -1));
}
TEST(ParseBoolFlag, DashToSomethingExact)
{
	ASSERT_FALSE(parse_bool_flag(dash, str_one, -1));
	ASSERT_FALSE(parse_bool_flag(str_one, dash, -1));
	ASSERT_FALSE(parse_bool_flag(dash, str_two, -1));
	ASSERT_FALSE(parse_bool_flag(str_two, dash, -1));
}
TEST(ParseBoolFlag, DashZero)
{
	ASSERT_TRUE(parse_bool_flag(dash, another_dash, 0));
	ASSERT_TRUE(parse_bool_flag(dash, dash, 0));
}
TEST(ParseBoolFlag, DashToSomethingZero)
{
	ASSERT_FALSE(parse_bool_flag(dash, str_one, 0));
	ASSERT_FALSE(parse_bool_flag(str_one, dash, 0));
	ASSERT_FALSE(parse_bool_flag(dash, str_two, 0));
	ASSERT_FALSE(parse_bool_flag(str_two, dash, 0));
}
TEST(ParseBoolFlag, DashToFirstChar)
{
	ASSERT_TRUE(parse_bool_flag(dash, another_dash, 1));
	ASSERT_TRUE(parse_bool_flag(dash, dash, 1));
}
TEST(ParseBoolFlag, DashToSomethingToFirstChar)
{
	ASSERT_FALSE(parse_bool_flag(dash, str_one, 1));
	ASSERT_FALSE(parse_bool_flag(str_one, dash, 1));
	ASSERT_FALSE(parse_bool_flag(dash, str_two, 1));
	ASSERT_FALSE(parse_bool_flag(str_two, dash, 1));
}
TEST(ParseBoolFlag, DashToDashieExact)
{
	ASSERT_FALSE(parse_bool_flag(dash, "-a", -1));
	ASSERT_FALSE(parse_bool_flag("-a", dash, -1));
}
TEST(ParseBoolFlag, DashToDashieFirstChar)
{
	ASSERT_TRUE(parse_bool_flag(dash, "-a", 1));
	ASSERT_FALSE(parse_bool_flag("-a", dash, 1));
}

TEST(LciCalledByRealName, NotApprovedNames)
{
	ASSERT_FALSE(lci_called_by_real_name(".."));
	ASSERT_FALSE(lci_called_by_real_name("."));
	ASSERT_FALSE(lci_called_by_real_name(""));
	ASSERT_FALSE(lci_called_by_real_name("/"));
	ASSERT_FALSE(lci_called_by_real_name("/lci/."));
	ASSERT_FALSE(lci_called_by_real_name("/lci/"));
	ASSERT_FALSE(lci_called_by_real_name("/usr/"));
	ASSERT_FALSE(lci_called_by_real_name("/usr/lib"));
	ASSERT_FALSE(lci_called_by_real_name("ci"));
	ASSERT_FALSE(lci_called_by_real_name("lc"));
	ASSERT_FALSE(lci_called_by_real_name("li"));
	ASSERT_FALSE(lci_called_by_real_name("usr"));
}

TEST(LciCalledByRealName, ApprovedNames)
{
	ASSERT_TRUE(lci_called_by_real_name("../lci"));
	ASSERT_TRUE(lci_called_by_real_name("./lci"));
	ASSERT_TRUE(lci_called_by_real_name("/lci"));
	ASSERT_TRUE(lci_called_by_real_name("/usr/lci"));
	ASSERT_TRUE(lci_called_by_real_name("lci"));
}

template<int N>
void TestRemoveIndex(char const* (&orig_argv)[N + 1], int const orig_offset,
		char const* const (&exp_argv)[N])
{
	int offset = orig_offset;
	int argc = N + 1;

	remove_index(&offset, &argc, const_cast<char**>(orig_argv));

	ASSERT_THAT(offset, Eq(orig_offset-1));
	ASSERT_THAT(argc, Eq(N));
	for (int i = 0; i != N - 1; ++i)
		EXPECT_THAT(orig_argv[i], Eq(exp_argv[i]));
	EXPECT_THAT(orig_argv[N-1], Eq(exp_argv[N-1]));
}

char const* const RandomString[4] = { "", "Aircraft Carrier", "Vampire",
		"Space Shuttle" };

TEST(RemoveIndex, MinimumCmdLineEmptyString)
{
	char const* argv[] = { "", NULL };
	char const* const expected_argv[] = { NULL };

	TestRemoveIndex(argv, 0, expected_argv);
}
TEST(RemoveIndex, MinimumCmdLineSomethingString)
{
	char const* argv[] = { RandomString[2], NULL };
	char const* const expected_argv[] = { NULL };

	TestRemoveIndex(argv, 0, expected_argv);
}
TEST(RemoveIndex, FirstElementRemoved)
{
	char const* argv[] = { RandomString[3], RandomString[1], NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };

	TestRemoveIndex(argv, 0, expected_argv);
}
TEST(RemoveIndex, SecondElementRemoved)
{
	char const* argv[] = { RandomString[2], RandomString[1], NULL };
	char const* const expected_argv[] = { RandomString[2], NULL };

	TestRemoveIndex(argv, 1, expected_argv);
}
