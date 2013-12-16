#ifndef _LCS_GUARD
#define _LCS_GUARD
#include "_string.h"
#include <stdbool.h>
#define max(a, b)((a) > (b) ? (a) : (b))

char** build_lcs_matrix(char** chaine_a, char** chaine_b, int size_a, int size_b, bool options);
void extract_lcs(char** matrix, char** a, char** b, int x, int y, char** lcs, bool options);
void display_lcs_matrix(char** matrix, int size_a, int size_b, char** chaine_a, char** chaine_b);

void display_matrix(char** matrix, int n, int m);
int get_size_of_lcs(char* a, char* b, int i, int j, int size_a, int size_b);


#endif // LCS

