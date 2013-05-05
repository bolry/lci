#define _GNU_SOURCE

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "core.h"

#define TOOL_NAME "lci"

static char const *version[] = {
	TOOL_NAME "",
	NULL
};

static char const *banner[] = {
	TOOL_NAME " copyright (c) 2013 bo rydberg",
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
	NULL
};

static int force_lint = 0;
static int run_compiler = 1;
static int run_lint = 1;
static int show_banner = 1;
static int verbose = 0;

static void fputa(char const *a[], FILE * s)
{
	int i;

	for (i = 0; a[i] != NULL; ++i)
		if (fprintf(s, "%s\n", a[i]) < 0)
			exit(EXIT_FAILURE);
}

static int lci_called_by_real_name(char const *path)
{
	char *bname;
	char *tmp_path;
	int same;

	tmp_path = strdup(path);
	if (NULL == tmp_path) {
		fputs(TOOL_NAME ": out of memory\n", stderr);
		exit(EXIT_FAILURE);
	}
	bname = basename(tmp_path);
	same = (strcmp(bname, TOOL_NAME) == 0);
	free(tmp_path);
	return same;
}

static void print_usage_on(FILE * s)
{
	fputa(usage, s);
}

static void print_version_on(FILE * s)
{
	fputa(version, s);
}

int parse_bool_flag(char const unknown_arg[], char const option[],
		    int unique_from)
{
	int match;

	if (unique_from < 0) {
		match = strcmp(unknown_arg, option) == 0;
	} else {
		char const *res = strstr(option, unknown_arg);
		if (res != option)
			match = 0;
		else
			match = strlen(unknown_arg) >= (size_t) unique_from;
	}
	return match;
}

static void remove_index(int *offset, int *acnt, char *avec[])
{
	int i;

	for (i = *offset; i != *acnt; ++i)
		avec[i] = avec[i + 1];
	--(*acnt);
	--(*offset);
}

static void lci_options(int *acnt, char *avec[])
{
	int i;

	if (*acnt < 2) {
		print_usage_on(stderr);
		exit(EXIT_FAILURE);
	}
	for (i = 1; i != *acnt; ++i) {
		if (parse_bool_flag(avec[i], "-b", -1) ||
		    parse_bool_flag(avec[i], "--no-banner", 6)) {
			fputs("no banner\n", stderr);
			show_banner = 0;
			remove_index(&i, acnt, avec);
			continue;
		}
		if (parse_bool_flag(avec[i], "-c", -1) ||
		    parse_bool_flag(avec[i], "--no-compiler", 6)) {
			fputs("no compiler\n", stderr);
			run_compiler = 0;
			remove_index(&i, acnt, avec);
			continue;
		}
		if (parse_bool_flag(avec[i], "-f", -1) ||
		    parse_bool_flag(avec[i], "--force-lint", 3)) {
			fputs("force lint\n", stderr);
			force_lint = 1;
			remove_index(&i, acnt, avec);
			continue;
		}
		if (parse_bool_flag(avec[i], "-l", -1) ||
		    parse_bool_flag(avec[i], "--no-lint", 6)) {
			fputs("no lint\n", stderr);
			run_lint = 0;
			remove_index(&i, acnt, avec);
			continue;
		}
		if (parse_bool_flag(avec[i], "-v", -1) ||
		    parse_bool_flag(avec[i], "--verbose", 6)) {
			fputs("verbose\n", stderr);
			++verbose;
			remove_index(&i, acnt, avec);
			continue;
		}
		if (parse_bool_flag(avec[i], "--help", 3)) {
			fputs("help\n", stderr);
			print_usage_on(stdout);
			exit(EXIT_SUCCESS);
		}
		if (parse_bool_flag(avec[i], "--version", 6)) {
			fputs("version\n", stderr);
			print_version_on(stdout);
			exit(EXIT_SUCCESS);
		}
		break;
	}
}

int lci_main(int argc, char *argv[])
{
	if (lci_called_by_real_name(argv[0]))
		lci_options(&argc, argv);
	if (show_banner)
		fputa(banner, stderr);
	if (run_compiler) {
		/*
		 * check compiler options for compiling and/or linking only,
		 * turn off lint if anything else.
		 */
	}

	if (fflush(NULL) != 0) {
		perror(TOOL_NAME ": fflush");
		exit(EXIT_FAILURE);
	}
	if (run_compiler && run_lint) {
		/*
		 * run compiler first and if OK then run lint
		 */
		int status;
		pid_t pid;
		pid_t wpid;

		pid = fork();
		if (-1 == pid) {
			perror(TOOL_NAME ": fork");
			exit(EXIT_FAILURE);
		}
		if (0 == pid) {
			(void)execvp(argv[1], &argv[1]);
			perror(TOOL_NAME ": execvp");
			_exit(EXIT_FAILURE);
		}
		wpid = waitpid(pid, &status, 0);
		if (wpid != pid) {
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
		argv[0] = "fl";
		(void)execvp(argv[0], &argv[0]);
		perror(TOOL_NAME ": execvp");
	} else if (run_compiler) {
		(void)execvp(argv[1], &argv[1]);
		perror(TOOL_NAME ": execvp");
	} else if (run_lint) {
		argv[0] = "fl";
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
