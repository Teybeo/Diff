#include "lcs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
    Alloue une matrice permettant ensuite de trouver le LCS de chaine_a et chaine_b
    Ne contient pas d'en-têtes
*/
char** build_lcs_matrix(char* chaine_a, char* chaine_b, int size_a, int size_b) {

    // +1 pour le caractère vide
    int size_x = size_a + 1; // Nombre de colonnes
    int size_y = size_b + 1; // Nombre de lignes
    int x, y;

    char** matrix = malloc(sizeof(char*) * size_y);

    for (y = 0 ; y < size_y ; y++ )
    {
        matrix[y] = malloc(sizeof(char) * size_x);
        memset(matrix[y], '0', sizeof(char) * size_x);
    }

//    for (y = 1 ; y < size_y ; y++ )
//    {
//        for (x = 1 ; x < size_x; x++ )
//        {
//            if (chaine_b[y-1] == chaine_a[x-1])
//                matrix[y][x] = 1 + matrix[y-1][x-1];
//            else
//                matrix[y][x] = max(matrix[y-1][x], matrix[y][x-1]);
//            display_lcs_matrix(matrix, size_a, size_b, chaine_a, chaine_b);
//        }
//    }
    for (y = 0 ; y < size_b ; y++ )
    {
        for (x = 0 ; x < size_a; x++ )
        {
            if (chaine_a[x] == chaine_b[y])
                matrix[y+1][x+1] = 1 + matrix[y][x];
            else
                matrix[y+1][x+1] = max(matrix[y][x+1], matrix[y+1][x]);
            display_lcs_matrix(matrix, size_a, size_b, chaine_a, chaine_b);
        }
    }

    return matrix;
}
/*
    Cette fonction extrait la (une) lcs des chaines a et b à partir de la matrice
*/
void extract_lcs(char** matrix, char* a, char* b, int x, int y, char* lcs)
{
    static int i = 0;

    if (x == 0 || y == 0)
        return;

    if (a[x - 1] == b[y - 1])
    {
        extract_lcs(matrix, a, b, x - 1, y - 1, lcs);
        lcs[i] = a[x - 1];
        i++;
    }
    else if (matrix[y][x] == matrix[y - 1][x])
        extract_lcs(matrix, a, b, x, y - 1, lcs); // On prend le chemin du haut
    else
        extract_lcs(matrix, a, b, x - 1, y, lcs); // On prend le chemin de gauche
}

/*
    Affiche la matrice en rajoutant les caractères des chaine a et b en en-têtes des lignes et colonnes
*/
void display_lcs_matrix(char** matrix, int size_a, int size_b, char* chaine_a, char* chaine_b) {

    int i, j;

    printf("    ");
    for (j = 0 ; j < size_a; j++ )
        printf("%c ", chaine_a[j]);

   putchar('\n');

    for (i = 0 ; i < size_b + 1 ; i++ )
    {
        printf("%c ", (i == 0) ? ' ' : chaine_b[i - 1]);

        for (j = 0 ; j < size_a + 1; j++ )
            printf("%c ", matrix[i][j]);

        putchar('\n');
    }

    putchar('\n');
}

void display_matrix(char** matrix, int n, int m) {

    int i, j;
    for (i = 0 ; i < n ; i++ )
    {
        for (j = 0 ; j < m; j++ )
            printf("%c ", matrix[i][j]);

        puts("");
    }
        puts("");
}

int get_size_of_lcs(char* a, char* b, int i, int j, int size_a, int size_b) {

    if (i >= size_a || j >= size_b)
        return 0;

    if (a[i] == b[j])
        return 1 + get_size_of_lcs(a, b, i+1, j+1, size_a, size_b);

    else
        return max(get_size_of_lcs(a, b, i+1, j, size_a, size_b), get_size_of_lcs(a, b, i, j+1, size_a, size_b));

}
