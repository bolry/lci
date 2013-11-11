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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

static enum severity severity_ceiling_ = LCI_SEV_NOTICE;
static FILE *log_stream_ = NULL;

int (*stream_format_output) (FILE * stream, char const *format, ...) = fprintf;

char *xstrdup(char const *str)
{
	char *dup = (char *)malloc(strlen(str) + 1u);
	if (NULL == dup) {
		log_puts(LCI_SEV_ALERT, "out-of-memory\n");
		fputs(TOOL_NAME ": out-of-memory\n", stderr);
		exit(EXIT_FAILURE);
	}
	(void)strcpy(dup, str);
	return dup;
}

int log_printf(enum severity severity, char const *format, ...)
{
	int ret;
	va_list ap;

	va_start(ap, format);
	ret = log_vprintf(severity, format, ap);
	va_end(ap);
	return ret;
}

int log_vprintf(enum severity severity, char const *format, va_list args)
{
	if (is_severity_logged(severity))
		if (log_stream_ != NULL)
			return vfprintf(log_stream_, format, args);
	return 0;
}

void log_puts(enum severity severity, char const *message)
{
	if (is_severity_logged(severity))
		if (log_stream_ != NULL)
			(void)fputs(message, log_stream_);
}

int is_severity_logged(enum severity severity)
{
	return (severity & 0xF) <= severity_ceiling_;
}

enum severity get_severity_ceiling(void)
{
	return severity_ceiling_;
}

enum severity set_severity_ceiling(enum severity ceiling)
{
	enum severity r = severity_ceiling_;
	severity_ceiling_ = ceiling;
	return r;
}

void inc_severity_ceiling(void)
{
	enum severity const severity = get_severity_ceiling();
	if (severity < LCI_SEV_DEBUG)
		set_severity_ceiling((enum severity)(severity + 1));
}
