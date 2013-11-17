#include "lcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit_list.h"

int main(int argc, char** argv)
{
    if (argc < 3) {
        puts("Nombre de parametres insuffisants");
        return 1;
    }
    char* chaine_a = argv[1];
    char* chaine_b = argv[2];
    int size_a = strlen(chaine_a);
    int size_b = strlen(chaine_b);

    char* lcs = calloc(sizeof(char), max(size_a, size_b) + 1);

//    int size_lcs = get_size_of_lcs(chaine_a, chaine_b, 0, 0);
//    printf("%d\n", size_lcs);

    char** matrix = build_lcs_matrix(chaine_a, chaine_b, size_a, size_b);
    extract_lcs(matrix, chaine_a, chaine_b, size_a, size_b, lcs);

    puts(lcs);

    Edit* addition = NULL;
    Edit* deletion = NULL;

    int size_lcs = strlen(lcs);

    int i = 0, j;
    for (j = 0 ; j < size_lcs ; j++ )
    {
        for ( ; i < size_a ; i++ )
        {
            if (chaine_a[i] != lcs[j])
                deletion = empiler(deletion, chaine_a[i]);
            else
            {
                i++;
                break;
            }
        }
    }

    print_list(deletion);

    return 0;
}



FILE* open_file(char* filename) {

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir '%s'\n", filename);
    }

    return file;
}

