#include <stdarg.h>
#include <stdio.h>
#include "inc/util.h"

static int severity_ceiling_ = LCI_SEV_NOTICE;
static FILE *log_stream_ = NULL;

int (*FormatOutput) (char const *format, ...) = printf;
int (*StreamFormatOutput) (FILE * stream, const char *format, ...) = fprintf;

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
	if (is_severity_logged(severity)) {
		if (NULL == log_stream_)
			log_stream_ = stderr;
		return vfprintf(log_stream_, format, args);
	}
	return 0;
}

void log_puts(enum severity_t severity, const char *message)
{
	if (is_severity_logged(severity)) {
		if (NULL == log_stream_)
			log_stream_ = stderr;
		fputs(message, log_stream_);
	}
}

int is_severity_logged(enum severity_t severity)
{
	return (severity & 0xF) <= severity_ceiling_;
}

int get_severity_ceiling(void)
{
	return severity_ceiling_;
}

int set_severity_ceiling(int ceiling)
{
	int r = severity_ceiling_;
	severity_ceiling_ = ceiling;
	return r;
}
