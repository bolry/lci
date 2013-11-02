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

#ifdef __cplusplus
extern "C" {
#endif

	enum severity_t {
		LCI_SEV_EMERGENCY = 0,	/*!< system is unusable */
		LCI_SEV_ALERT = 1,	/*!< action must be taken immediately */
		LCI_SEV_CRITICAL = 2,	/*!< critical conditions */
		LCI_SEV_ERROR = 3,	/*!< error conditions */
		LCI_SEV_WARNING = 4,	/*!< warning conditions */
		LCI_SEV_NOTICE = 5,	/*!< normal but significant condition */
		LCI_SEV_INFORMATIONAL = 6,	/*!< informational */
		LCI_SEV_DEBUG = 7	/*!< debug-level messages */
	};

	extern int log_printf(enum severity_t severity, char const *format, ...
	    );
	extern int log_vprintf(enum severity_t severity,
			      char const *format, va_list args);
	extern void log_puts(enum severity_t severity, char const *message);

	extern int (*format_output) (const char *format, ...);
	extern int (*stream_format_output) (FILE * stream, const char *format,
					    ...);

#ifdef __cplusplus
}
#endif
