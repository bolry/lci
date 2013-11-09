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

#ifndef LCI_INC_CORE_H_
#define LCI_INC_CORE_H_
#else
#error "LCI_INC_CORE_H_"
#endif

extern int force_lint;
extern int run_compiler;
extern int run_lint;
extern int show_banner;

void lci_options(int *cnt, char *vec[]);
int lci_called_by_real_name(char const *path);
int parse_bool_flag(char const unknown_arg[], char const option[],
		    int unique_from);
void remove_index(int *offset, int *cnt, char *vec[]);
