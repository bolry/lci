#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/util.h"

static enum severity_t severity_ceiling_ = LCI_SEV_NOTICE;
static FILE *log_stream_ = NULL;

int (*format_output) (char const *format, ...) = printf;
int (*stream_format_output) (FILE * stream, const char *format, ...) = fprintf;

char *xstrdup(char const *str)
{
	char *dup = malloc(strlen(str) + 1u);
	if (NULL == dup) {
		log_puts(LCI_SEV_ALERT, "out-of-memory\n");
		fputs(TOOL_NAME ": out-of-memory\n", stderr);
		exit(EXIT_FAILURE);
	}
	(void)strcpy(dup, str);
	return dup;
}

int log_printf(enum severity_t severity, const char *format, ...)
{
	int ret;
	va_list ap;

	va_start(ap, format);
	ret = log_vprintf(severity, format, ap);
	va_end(ap);
	return ret;
}

int log_vprintf(enum severity_t severity, const char *format, va_list args)
{
	if (is_severity_logged(severity))
		if (log_stream_ != NULL)
			return vfprintf(log_stream_, format, args);
	return 0;
}

void log_puts(enum severity_t severity, const char *message)
{
	if (is_severity_logged(severity))
		if (log_stream_ != NULL)
			(void)fputs(message, log_stream_);
}

int is_severity_logged(enum severity_t severity)
{
	return (severity & 0xF) <= severity_ceiling_;
}

enum severity_t get_severity_ceiling(void)
{
	return severity_ceiling_;
}

enum severity_t set_severity_ceiling(enum severity_t ceiling)
{
	enum severity_t r = severity_ceiling_;
	severity_ceiling_ = ceiling;
	return r;
}
