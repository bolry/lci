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

#include <errno.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "core.h"
#include "util.h"

#define CANONICAL_TOOL_NAME "Lint Compiler Interceptor"
#define COPYRIGHT_STRING "Copyright (c) 2013 Bo Rydberg"

static char lint[] = "fake-lint-nt.exe";

static char const *copyright[] = {
	COPYRIGHT_STRING,
	"License GPLv2: GNU GPL version 2 or later <http://gnu.org/licenses/>",
	"This is free software: you are free to change and redistribute it.",
	"There is NO WARRANTY, to the extent permitted by law.",
	NULL
};

static char const *version[] = {
	CANONICAL_TOOL_NAME " 0.0",
	NULL
};

static char const *banner[] = {
	CANONICAL_TOOL_NAME " " COPYRIGHT_STRING,
	NULL
};

static char const *usage[] = {
	"usage:",
	"    " TOOL_NAME " [options]",
	"    " TOOL_NAME " [options] compiler [compiler options]",
	"    compiler [compiler options]    (via symbolic link)",
	"",
	"options:",
	"    -b, --no-banner    suppress banner",
	"    -c, --no-compiler  do not run compiler",
	"    -f, --force-lint   run lint even after failed compile",
	"    -l, --no-lint      do not run lint",
	"    -v, --verbose      verbose output",
	"",
	"        --help         print this text and exit",
	"        --version      print version and exit",
	"",
	"Report bugs to: mailing-address",
	CANONICAL_TOOL_NAME " home page: <https://github.com/bolry/lci/>",
	NULL
};

int force_lint = 0;
int run_compiler = 1;
int run_lint = 1;
int show_banner = 1;

static void fputa(char const *arr[], FILE * stream)
{
	int i;

	for (i = 0; arr[i] != NULL; ++i)
		if (stream_format_output(stream, "%s\n", arr[i]) < 0) {
			log_puts(LCI_SEV_ALERT, "cannot print\n");
			exit(EXIT_FAILURE);
		}
}

int lci_called_by_real_name(char const *path)
{
	char *tmp_path;
	char const *bname;
	int same;

	tmp_path = xstrdup(path);
	bname = basename(tmp_path);
	same = (strcmp(bname, TOOL_NAME) == 0);
	free(tmp_path);
	/*
	 * Trailing slash check because basename can remove it
	 */
	if (same)
		same = (path[strlen(path) - 1u] != '/');
	return same;
}

static void print_usage_on(FILE * stream)
{
	fputa(usage, stream);
}

static void print_version_on(FILE * stream)
{
	fputa(version, stream);
	fputa(copyright, stream);
}

int parse_bool_flag(char const unknown_arg[], char const option[],
		    int unique_from)
{
	int match;

	if (unique_from < 0) {
		log_puts(LCI_SEV_DEBUG, "exact match requested\n");
		match = (strcmp(unknown_arg, option) == 0);
	} else {
		char const *const res = strstr(option, unknown_arg);
		log_printf(LCI_SEV_DEBUG, "first %d must match\n", unique_from);
		if (res != option)
			match = 0;
		else
			match = (strlen(unknown_arg) >= (size_t) unique_from);
	}
	return match;
}

void remove_index(int *offset, int *cnt, char *vec[])
{
	/*
	 * memmove does this
	 *     for (int i = *offset; i < *cnt; ++i)
	 *             vec[i] = vec[i + 1];
	 */
	memmove(&vec[*offset], &vec[*offset + 1],
		sizeof(char *) * (*cnt - *offset));
	--(*cnt);
	--(*offset);
}

void lci_options(int *cnt, char *vec[])
{
	int i;

	if (*cnt < 2) {
		print_usage_on(stderr);
		exit(EXIT_FAILURE);
	}
	for (i = 1; i != *cnt; ++i) {
		if (parse_bool_flag(vec[i], "-b", -1) ||
		    parse_bool_flag(vec[i], "--no-banner", 6)) {
			log_puts(LCI_SEV_DEBUG, "no banner\n");
			show_banner = 0;
			remove_index(&i, cnt, vec);
			continue;
		}
		if (parse_bool_flag(vec[i], "-c", -1) ||
		    parse_bool_flag(vec[i], "--no-compiler", 6)) {
			log_puts(LCI_SEV_DEBUG, "no compiler\n");
			run_compiler = 0;
			remove_index(&i, cnt, vec);
			continue;
		}
		if (parse_bool_flag(vec[i], "-f", -1) ||
		    parse_bool_flag(vec[i], "--force-lint", 3)) {
			log_puts(LCI_SEV_DEBUG, "force lint\n");
			force_lint = 1;
			remove_index(&i, cnt, vec);
			continue;
		}
		if (parse_bool_flag(vec[i], "-l", -1) ||
		    parse_bool_flag(vec[i], "--no-lint", 6)) {
			log_puts(LCI_SEV_DEBUG, "no lint\n");
			run_lint = 0;
			remove_index(&i, cnt, vec);
			continue;
		}
		if (parse_bool_flag(vec[i], "-v", -1) ||
		    parse_bool_flag(vec[i], "--verbose", 6)) {
			log_puts(LCI_SEV_DEBUG, "verbose\n");
			inc_severity_ceiling();
			remove_index(&i, cnt, vec);
			continue;
		}
		if (parse_bool_flag(vec[i], "--help", 3)) {
			log_puts(LCI_SEV_DEBUG, "help\n");
			print_usage_on(stdout);
			exit(EXIT_SUCCESS);
		}
		if (parse_bool_flag(vec[i], "--version", 6)) {
			log_puts(LCI_SEV_DEBUG, "version\n");
			print_version_on(stdout);
			exit(EXIT_SUCCESS);
		}
		break;
	}
}

int will_compile_and_or_link(int argc, char *argv[])
{
	return 1;
}

static void print_banner(void)
{
	if (show_banner)
		fputa(banner, stderr);
}

static void flush_all(void)
{
	if (fflush(NULL) == EOF) {
		perror(TOOL_NAME ": fflush");
		exit(EXIT_FAILURE);
	}
}

static void handle_possible_lci_options(int *argc, char *argv[])
{
	if (lci_called_by_real_name(argv[0]))
		lci_options(argc, &argv[0]);
}

static void only_run_lint_if_compile_and_or_link(int argc, char *argv[])
{
	if (run_compiler)
		if (!will_compile_and_or_link(argc, &argv[0]))
			run_lint = 0;
}

int lci_main(int argc, char *argv[])
{
	handle_possible_lci_options(&argc, &argv[0]);
	print_banner();
	only_run_lint_if_compile_and_or_link(argc, &argv[0]);
	flush_all();
	if (run_compiler && run_lint) {
		/*
		 * run compiler first and if OK then run lint
		 */
		int status;
		pid_t cpid;
		pid_t w;

		cpid = fork();
		if (-1 == cpid) {
			perror(TOOL_NAME ": fork");
			exit(EXIT_FAILURE);
		}
		if (0 == cpid) {
			(void)execvp(argv[1], &argv[1]);
			perror(TOOL_NAME ": execvp");
			_exit(EXIT_FAILURE);
		}
		errno = 0;
		w = waitpid(cpid, &status, 0);
		if (w != cpid) {
			perror(TOOL_NAME ": waitpid");
			exit(EXIT_FAILURE);
		}
		if (WIFEXITED(status) && (WEXITSTATUS(status) != EXIT_SUCCESS)) {
			if (force_lint) {
				/*
				 * run lint anyway
				 */
			} else {
				exit(WEXITSTATUS(status));
			}
		}
		if (WIFSIGNALED(status))
			exit(WTERMSIG(status));
		argv[0] = lint;
		(void)execvp(argv[0], &argv[0]);
		perror(TOOL_NAME ": execvp");
	} else if (run_compiler) {
		(void)execvp(argv[1], &argv[1]);
		perror(TOOL_NAME ": execvp");
	} else if (run_lint) {
		argv[0] = lint;
		(void)execvp(argv[0], &argv[0]);
		perror(TOOL_NAME ": execvp");
	} else {
		/*
		 * a do nothing option
		 */
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}
