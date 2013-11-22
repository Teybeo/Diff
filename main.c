#include "lcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit_list.h"

int main(int argc, char** argv)
{

#ifdef ARGV_INPUT
    if (argc < 3) {
        puts("Nombre de parametres insuffisants");
        return 1;
    }
    char* chaine_a = argv[1];
    char* chaine_b = argv[2];
#else
    char chaine_a[256] = {};
    char chaine_b[256] = {};
    fgets(chaine_a, 256, stdin);
    fgets(chaine_b, 256, stdin);
    chaine_a[strlen(chaine_a)-1] = '\0';
    chaine_b[strlen(chaine_b)-1] = '\0';
#endif

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

    // Rappel: Si un caractère est dans le lcs, alors il est obligatoirement présent dans la chaine de départ ET d'arrivée
    /*  Pour chaque caractère du lcs (le noyau dur) et dans l'ordre
            On cherche ce caractère dans la chaine a en partant de la dernière position où un caractère a été trouvé (0 au début)
                Dès qu'on trouve, on arrête de chercher ce caractère
                Pour chaque position où ne trouve pas, on marque le caractère de a à cette position comme ayant été supprimé
            Même chose dans b sauf qu'on marque les caractères des positions non-trouvées comme des additions
    */
    int i, j = 0, k = 0;
    for (i = 0 ; i < size_lcs ; i++ )
    {
        for ( ; j < size_a ; j++ )
        {
            if (chaine_a[j] == lcs[i])
            {
                j++;
                break;
            }
            else
                deletion = empilerFin(deletion, chaine_a[j], j);
        }

        for ( ; k < size_b ; k++ )
        {
            if (chaine_b[k] == lcs[i])
            {
                k++;
                break;
            }
            else
                addition = empilerFin(addition, chaine_b[k], k);
        }
    }

    puts("Suppression:");
    print_list(deletion);
    puts("Addition:");
    print_list(addition);

    return 0;
}



FILE* open_file(char* filename) {

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir '%s'\n", filename);
    }

    return file;
}

