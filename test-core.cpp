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

extern "C" {
#include <stdarg.h>
#include <stdio.h>
#include "core.h"
#include "util.h"
}

#include <gmock/gmock.h>

#define ARGV_COUNT(x) ((int)((int)sizeof(x) / (int)sizeof(*x)))

using namespace testing;

/*
 * Defining array with same content, since it is implementation defined it
 * duplicate strings occupy the same memory or not.
 */
char const empty[0 + 1] = "";
char const another_empty [0 + 1] = "";
char const dash[] = "-";
char const another_dash[] = "-";
char const str_one[1 + 1] = "a";
char const str_two[2 + 1] = "bc";

char const usage_regex[] = "usage:.*options:.*bugs";

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

	ASSERT_THAT(offset, Eq(orig_offset - 1));
	ASSERT_THAT(argc, Eq(N));
	/*
	 * Yes, we want to compare the pointer addresses, no StrEq
	 */
	for (int i = 0; i != N; ++i)
		EXPECT_THAT(orig_argv[i], Eq(exp_argv[i]));
}

char const* const RandomString[3] = {
		"Aircraft Carrier", "Vampire", "Space Shuttle"
};

TEST(RemoveIndex, MinimumCmdLineEmptyString)
{
	char const* argv[] = { "", NULL };
	char const* const expected_argv[] = { NULL };

	TestRemoveIndex(argv, 0, expected_argv);
}

TEST(RemoveIndex, MinimumCmdLineSomethingString)
{
	char const* argv[] = { RandomString[1], NULL };
	char const* const expected_argv[] = { NULL };

	TestRemoveIndex(argv, 0, expected_argv);
}

TEST(RemoveIndex, FirstElementRemoved)
{
	char const* argv[] = { RandomString[2], RandomString[0], NULL };
	char const* const expected_argv[] = { RandomString[0], NULL };

	TestRemoveIndex(argv, 0, expected_argv);
}

TEST(RemoveIndex, MiddleElementRemoved)
{
	char const* argv[] = { RandomString[2], RandomString[1], RandomString[0], NULL };
	char const* const expected_argv[] = { RandomString[2], RandomString[0], NULL };

	TestRemoveIndex(argv, 1, expected_argv);
}

TEST(RemoveIndex, LastElementRemoved)
{
	char const* argv[] = { RandomString[1], RandomString[0], NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };

	TestRemoveIndex(argv, 1, expected_argv);
}

TEST(LciOptionsDeathTest, TooShortCmdLine)
{
	char *argv[] = { NULL };
	int argc = ARGV_COUNT(argv) - 1;

	ASSERT_DEATH(lci_options(&argc, argv), usage_regex);
}

TEST(LciOptionsDeathTest, ShortCmdLine)
{
	char const *argv[] = { RandomString[0], NULL };
	int argc = ARGV_COUNT(argv) - 1;

	ASSERT_DEATH(lci_options(&argc, (char**)argv), usage_regex);
}

template<int M, int N>
void TestLciOptions(char const* (&orig_argv)[M], char const* const (&exp_argv)[N])
{
	int argc = M - 1;

	lci_options(&argc, const_cast<char**>(orig_argv));

	ASSERT_THAT(argc, Eq(N - 1));
	for (int i = 0; i != N - 1; ++i)
		EXPECT_THAT(orig_argv[i], Eq(exp_argv[i]));
}

TEST(LciOptions, BannerOptionShort)
{
	char const* argv[] = { RandomString[1], "-b", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_banner = show_banner;
	show_banner = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(show_banner);

	show_banner = old_banner;
}

TEST(LciOptions, BannerOptionLong)
{
	char const* argv[] = { RandomString[1], "--no-banner", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_banner = show_banner;
	show_banner = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(show_banner);

	show_banner = old_banner;
}

TEST(LciOptions, BannerOptionJustLongEnough)
{
	char const* argv[] = { RandomString[1], "--no-b", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_banner = show_banner;
	show_banner = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(show_banner);

	show_banner = old_banner;
}

TEST(LciOptions, CompilerOptionLong)
{
	char const* argv[] = { RandomString[1], "--no-compiler", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_compiler = run_compiler;
	run_compiler = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(run_compiler);

	run_compiler = old_compiler;
}

TEST(LciOptions, CompilerOptionJustLongEnough)
{
	char const* argv[] = { RandomString[1], "--no-c", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_compiler = run_compiler;
	run_compiler = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(run_compiler);

	run_compiler = old_compiler;
}

TEST(LciOptions, CompilerOptionShort)
{
	char const* argv[] = { RandomString[1], "-c", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_compiler = run_compiler;
	run_compiler = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(run_compiler);

	run_compiler = old_compiler;
}

TEST(LciOptions, ForceLintOptionLong)
{
	char const* argv[] = { RandomString[1], "--force-lint", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_force_lint = force_lint;
	force_lint = false;

	TestLciOptions(argv, expected_argv);
	EXPECT_TRUE(force_lint);

	force_lint = old_force_lint;
}

TEST(LciOptions, ForceLintOptionJustLongEnough)
{
	char const* argv[] = { RandomString[1], "--f", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_force_lint = force_lint;
	force_lint = false;

	TestLciOptions(argv, expected_argv);
	EXPECT_TRUE(force_lint);

	force_lint = old_force_lint;
}

TEST(LciOptions, ForceLintOptionShort)
{
	char const* argv[] = { RandomString[1], "-f", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_force_lint = force_lint;
	force_lint = false;

	TestLciOptions(argv, expected_argv);
	EXPECT_TRUE(force_lint);

	force_lint = old_force_lint;
}

TEST(LciOptions, RunLintOptionLong)
{
	char const* argv[] = { RandomString[1], "--no-lint", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_run_lint = run_lint;
	run_lint = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(run_lint);
	run_lint = old_run_lint;
}

TEST(LciOptions, RunLintOptionJustLongEnough)
{
	char const* argv[] = { RandomString[1], "--no-l", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_run_lint = run_lint;
	run_lint = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(run_lint);
	run_lint = old_run_lint;
}

TEST(LciOptions, RunLintOptionShort)
{
	char const* argv[] = { RandomString[1], "-l", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	int const old_run_lint = run_lint;
	run_lint = true;

	TestLciOptions(argv, expected_argv);
	EXPECT_FALSE(run_lint);
	run_lint = old_run_lint;
}

TEST(LciOptions, VerboseOptionLong)
{
	char const* argv[] = { RandomString[1], "--verbose", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	enum severity const old_severity = set_severity_ceiling(LCI_SEV_NOTICE);

	TestLciOptions(argv, expected_argv);
	ASSERT_THAT(get_severity_ceiling(), Eq(LCI_SEV_INFORMATIONAL));
	(void)set_severity_ceiling(old_severity);
}

TEST(LciOptions, VerboseOptionJustLongEnough)
{
	char const* argv[] = { RandomString[1], "--verb", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	enum severity const old_severity = set_severity_ceiling(LCI_SEV_NOTICE);

	TestLciOptions(argv, expected_argv);
	ASSERT_THAT(get_severity_ceiling(), Eq(LCI_SEV_INFORMATIONAL));
	(void)set_severity_ceiling(old_severity);
}

TEST(LciOptions, VerboseOptionShort)
{
	char const* argv[] = { RandomString[1], "-v", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	enum severity const old_severity = set_severity_ceiling(LCI_SEV_NOTICE);

	TestLciOptions(argv, expected_argv);
	ASSERT_THAT(get_severity_ceiling(), Eq(LCI_SEV_INFORMATIONAL));
	(void)set_severity_ceiling(old_severity);
}

TEST(LciOptions, TwoVerboseOptionGivesHigherLevel)
{
	char const* argv[] = { RandomString[1], "-v", "--verbose", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	enum severity const old_severity = set_severity_ceiling(LCI_SEV_NOTICE);

	TestLciOptions(argv, expected_argv);
	ASSERT_THAT(get_severity_ceiling(), Eq(LCI_SEV_DEBUG));
	(void)set_severity_ceiling(old_severity);
}

TEST(LciOptions, ManyVerboseOptionsHitsCeiling)
{
	char const* argv[] = { RandomString[1], "--verb", "--verbo", "--verbos", NULL };
	char const* const expected_argv[] = { RandomString[1], NULL };
	enum severity const old_severity = set_severity_ceiling(LCI_SEV_NOTICE);

	TestLciOptions(argv, expected_argv);
	ASSERT_THAT(get_severity_ceiling(), Eq(LCI_SEV_DEBUG));
	(void)set_severity_ceiling(old_severity);
}
