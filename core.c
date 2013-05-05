#define _GNU_SOURCE

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
	"Usage:",
	"    " TOOL_NAME " [options]",
	"    " TOOL_NAME " [options] compiler [compiler options]",
	"    compiler [compiler options]    (via symbolic link)",
	"",
	"Options:",
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

	for (i = 0; a[i] != NULL; ++i) {
		if (fprintf(s, "%s\n", a[i]) < 0)
			exit(EXIT_FAILURE);
	}
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

static int parse_bool_flag(char const *arg, char const *option, int unique_from)
{
	if (unique_from < 0)
		return strcmp(option, arg) == 0;
	if (strncmp(option, arg, unique_from) == 0) {
		;
	}
	return 1;
}

static void lci_options(int *c, char *v[])
{
	int i;

	if (*c < 2) {
		print_usage_on(stderr);
		exit(EXIT_FAILURE);
	}
	for (i = 1; i != *c; ++i) {
		if (parse_bool_flag(v[i], "-b", -1)
		    || parse_bool_flag(v[i], "--no-banner", 6)) {
			int j;

			fputs("No banner\n", stderr);
			show_banner = 0;
			for (j = i; j != *c; ++j)
				v[j] = v[j + 1];
			--(*c);
			--i;
			continue;
		}
		if (strcmp(v[i], "-c") == 0 ||
		    strcmp(v[i], "--no-c") == 0 ||
		    strcmp(v[i], "--no-co") == 0 ||
		    strcmp(v[i], "--no-com") == 0 ||
		    strcmp(v[i], "--no-comp") == 0 ||
		    strcmp(v[i], "--no-compi") == 0 ||
		    strcmp(v[i], "--no-compil") == 0 ||
		    strcmp(v[i], "--no-compile") == 0 ||
		    strcmp(v[i], "--no-compiler") == 0) {
			fputs("No compiler\n", stderr);
			run_compiler = 0;
			continue;
		}
		if (strcmp(v[i], "-f") == 0 ||
		    strcmp(v[i], "--f") == 0 ||
		    strcmp(v[i], "--fo") == 0 ||
		    strcmp(v[i], "--for") == 0 ||
		    strcmp(v[i], "--forc") == 0 ||
		    strcmp(v[i], "--force") == 0 ||
		    strcmp(v[i], "--force-") == 0 ||
		    strcmp(v[i], "--force-l") == 0 ||
		    strcmp(v[i], "--force-li") == 0 ||
		    strcmp(v[i], "--force-lin") == 0 ||
		    strcmp(v[i], "--force-lint") == 0) {
			fputs("Force lint\n", stderr);
			force_lint = 1;
			continue;
		}
		if (strcmp(v[i], "-l") == 0 ||
		    strcmp(v[i], "--no-l") == 0 ||
		    strcmp(v[i], "--no-li") == 0 ||
		    strcmp(v[i], "--no-lin") == 0 ||
		    strcmp(v[i], "--no-lin") == 0 ||
		    strcmp(v[i], "--no-lint") == 0) {
			fputs("No lint\n", stderr);
			run_lint = 0;
			continue;
		}
		if (strcmp(v[i], "-v") == 0 ||
		    strcmp(v[i], "--verb") == 0 ||
		    strcmp(v[i], "--verbo") == 0 ||
		    strcmp(v[i], "--verbos") == 0 ||
		    strcmp(v[i], "--verbose") == 0) {
			fputs("Verbose\n", stderr);
			++verbose;
			continue;
		}
		if (strcmp(v[i], "--h") == 0 ||
		    strcmp(v[i], "--he") == 0 ||
		    strcmp(v[i], "--hel") == 0 || strcmp(v[i], "--help") == 0) {
			fputs("Help\n", stderr);
			print_usage_on(stdout);
			exit(EXIT_SUCCESS);
		}
		if (strcmp(v[i], "--vers") == 0 ||
		    strcmp(v[i], "--versi") == 0 ||
		    strcmp(v[i], "--versio") == 0 ||
		    strcmp(v[i], "--version") == 0) {
			fputs("Version\n", stderr);
			print_version_on(stdout);
			exit(EXIT_SUCCESS);
		}

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
		 * check compiler options of compiling and linking
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
		pid_t wid;

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
		wid = waitpid(pid, &status, 0);
		if (wid != pid) {
			perror(TOOL_NAME ": waitpid");
			exit(EXIT_FAILURE);
		}
		if (WIFEXITED(status)
		    && (WEXITSTATUS(status) != EXIT_SUCCESS)) {
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
