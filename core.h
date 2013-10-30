#ifndef CORE_H
#define CORE_H
#else
#error "CORE_H"
#endif

#ifdef __cplusplus
extern "C" {
#endif

	int parse_bool_flag(char const unknown_arg[], char const option[],
			    int unique_from);

#ifdef __cplusplus
}
#endif
