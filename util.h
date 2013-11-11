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

#ifndef LCI_INC_UTIL_H_
#define LCI_INC_UTIL_H_
#else
#error "LCI_INC_UTIL_H_"
#endif

#define TOOL_NAME "lci"

enum severity {
	LCI_SEV_EMERGENCY,	/*!< system is unusable */
	LCI_SEV_ALERT,		/*!< action must be taken immediately */
	LCI_SEV_CRITICAL,	/*!< critical conditions */
	LCI_SEV_ERROR,		/*!< error conditions */
	LCI_SEV_WARNING,	/*!< warning conditions */
	LCI_SEV_NOTICE,		/*!< normal but significant condition */
	LCI_SEV_INFORMATIONAL,	/*!< informational */
	LCI_SEV_DEBUG		/*!< debug-level messages */
};

extern enum severity get_severity_ceiling(void);
extern enum severity set_severity_ceiling(enum severity ceiling);
extern int is_severity_logged(enum severity severity);
extern void inc_severity_ceiling(void);
extern int log_printf(enum severity severity, char const *format, ...);
extern int log_vprintf(enum severity severity, char const *format,
		       va_list args);
extern void log_puts(enum severity severity, char const *message);

extern int (*stream_format_output) (FILE * stream, char const *format, ...);
extern char *xstrdup(char const *s);
