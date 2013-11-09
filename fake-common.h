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

#include <stdio.h>
#include <stdlib.h>

static int is_only_known_lint_options(int argc, char *argv[])
{
	return 1;
}

int main(int argc, char *argv[])
{
	int i;
	if (printf("This is `%s'\n", FAKE_TOOL_NAME) < 0)
		return EXIT_FAILURE;
	for (i = 0; i != argc; ++i)
		if (printf("argv[%d]: `%s'\n", i, argv[i]) < 0)
			return EXIT_FAILURE;
	if (printf("End of `%s'\n", FAKE_TOOL_NAME) < 0)
		return EXIT_FAILURE;
	if (!is_only_known_lint_options(argc, argv))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
