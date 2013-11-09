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
