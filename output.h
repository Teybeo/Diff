#ifndef _OUTPUT_GUARD
#define _OUTPUT_GUARD

#include "options.h"

void print_diff_normal(char** file_a, int size_a, char** file_b, int size_b, char** lcs, int size_lcs);
void print_diff_unified(char** file_a, int size_a, char** file_b, int size_b, char** lcs, int size_lcs, Options* options);

#endif // OUTPUT

