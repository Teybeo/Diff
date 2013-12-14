#include "output.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ADDITION 1
#define DELETION 2
#define MODIFICATION 3

void print_edit(char** file_a, char** file_b, int idx_a, int idx_b, int nb_diff_a, int nb_diff_b, int edit_type);
void print_lines(char** file, int start, int count, char symbol);
void print_range(int start, int count);
int get_nb_lines_until_found(char** file, char* string, int start, int size);

// Rappel: Si un caractère est dans le lcs, alors il est obligatoirement présent dans la chaine de départ ET d'arrivée
/*  Pour chaque élément du lcs et dans l'ordre
        Si on ne trouve pas cet élément lcs dans B mais en le trouvant dans A, c'est que des lignes ont été ajoutées avant dans B
            On indique toutes ces lignes comme des ajouts et on les passe
        Si on ne trouve pas cet élément lcs dans A mais en le trouvant dans B, c'est que des lignes ont été supprimées dans B
            On indique toutes ces lignes comme des suppressions et on les passe
        Si on ne trouve cet élément lcs ni dans A ni dans B, c'est que ces lignes ont été changées de A en celles de B
            On indique toutes ces lignes comme changées et on les passe
*/
void print_diff_normal(char** file_a, int size_a, char** file_b, int size_b, char** lcs, int size_lcs) {

    bool found_in_a = true;
    bool found_in_b = true;
    int i, idx_a = 0, idx_b = 0;
    int nb_lines_different_a;
    int nb_lines_different_b;
    int edit_type = -1;

    for (i = 0 ; i < size_lcs ; i++ )
    {
        nb_lines_different_a = 0;
        nb_lines_different_b = 0;

        found_in_a = !strcmp(lcs[i], file_a[idx_a]);
        found_in_b = !strcmp(lcs[i], file_b[idx_b]);

        if (!found_in_a || !found_in_b)
        {
            nb_lines_different_a = get_nb_lines_until_found(file_a, lcs[i], idx_a, size_a);
            nb_lines_different_b = get_nb_lines_until_found(file_b, lcs[i], idx_b, size_b);

            // Un peu de magie pour déterminer le type de modif
            edit_type = !found_in_b + !found_in_a * 2;

            print_edit(file_a, file_b, idx_a, idx_b, nb_lines_different_a, nb_lines_different_b, edit_type);
        }

        // On passe les lignes différentes qu'on a trouvées
        idx_b += nb_lines_different_b;
        idx_a += nb_lines_different_a;
        // Puis on passe la ligne lcs qu'on cherchait
        idx_a++;
        idx_b++;
    }

    /* La boucle précédente s'arrête quand la dernière ligne lcs a été trouvée
    Ici on traite donc les éventuelles modifications faites en fin de fichier, après la dernière ligne lcs*/

    if (idx_a < size_a || idx_b < size_b)
    {
        nb_lines_different_a = size_a - idx_a;
        nb_lines_different_b = size_b - idx_b;

        edit_type = (idx_b < size_b) + (idx_a < size_a) * 2;

        print_edit(file_a, file_b, idx_a, idx_b, nb_lines_different_a, nb_lines_different_b, edit_type);
    }

}


void print_edit(char** file_a, char** file_b, int idx_a, int idx_b, int nb_diff_a, int nb_diff_b, int edit_type) {

    char edit_code[] = {'a', 'd', 'c'};

    print_range(idx_a, nb_diff_a);
    putchar(edit_code[edit_type]);
    print_range(idx_b, nb_diff_b);

    putchar('\n');

    if (edit_type == ADDITION)
        print_lines(file_b, idx_b, nb_diff_b, '>');
    else if (edit_type == DELETION)
        print_lines(file_a, idx_a, nb_diff_a, '<');
    else if (edit_type == MODIFICATION)
    {
        print_lines(file_a, idx_a, nb_diff_a, '<');
        puts("---");
        print_lines(file_b, idx_b, nb_diff_b, '>');
    }

}

int get_nb_lines_until_found(char** file, char* string, int start, int size) {

    int i;
    for (i = 0 ; i < size && strcmp(file[i + start], string) != 0 ; i++ );

    return i;

}
void print_lines(char** file, int start, int count, char symbol) {

    int i;
    for (i = 0 ; i < count ; i++ )
        printf("%c %s", symbol, file[start + i]);
}

void print_range(int start, int count) {

    if (count == 0)
        printf("%d", start); // Ligne après laquelle une modif a été faite
    else if (count == 1)
        printf("%d", start + 1); // Numéro de ligne réel
    else
        printf("%d,%d", start + 1, start + count);
}

void ignore_casse(char** file_a, int size_a, char** file_b, int size_b, char** lcs, int size_lcs){
    bool found_in_a = true;
    bool found_in_b = true;
    int i, idx_a = 0, idx_b = 0;
    int nb_lines_different_a;
    int nb_lines_different_b;

    for (i = 0 ; i < size_lcs ; i++ )
    {
        nb_lines_different_a = 0;
        nb_lines_different_b = 0;

        found_in_a = !strcmp(lcs[i], file_a[idx_a]);
        found_in_b = !strcmp(lcs[i], file_b[idx_b]);

        // Ligne lcs trouvée en idx_a dans A mais pas en idx_b dans B
        // => des lignes sont présentes dans B devant la ligne lcs, (ajoutées dans B)
        if (found_in_a && !found_in_b)
        {
            nb_lines_different_b = get_nb_lines_until_found(file_b, lcs[i], idx_b, size_b);
            if(diff_strcasecmp(file_a[idx_a], file_b[idx_b]) != 0)
                print_edit(file_a, file_b, idx_a, idx_b, 0, nb_lines_different_b, ADDITION);
        }
        // Ligne lcs trouvée en idx_b dans B mais pas en idx_a dans A
        // => des lignes sont présentes dans A devant la ligne lcs, (supprimées dans B)
        if (!found_in_a && found_in_b)
        {
            nb_lines_different_a = get_nb_lines_until_found(file_a, lcs[i], idx_a, size_a);
            if(diff_strcasecmp(file_a[idx_a], file_b[idx_b]) != 0)
                print_edit(file_a, file_b, idx_a, idx_b, nb_lines_different_a, 0, DELETION);
        }
        // Ligne lcs trouvée ni en idx_a dans A ni en idx_b dans B
        // => des lignes sont présentes dans A et dans B devant la ligne lcs, (changées de A à B)
        if (!found_in_a && !found_in_b)
        {
            nb_lines_different_a = get_nb_lines_until_found(file_a, lcs[i], idx_a, size_a);
            nb_lines_different_b = get_nb_lines_until_found(file_b, lcs[i], idx_b, size_b);
            if(diff_strcasecmp(file_a[idx_a], file_b[idx_b]) != 0)
                print_edit(file_a, file_b, idx_a, idx_b, nb_lines_different_a, nb_lines_different_b, MODIFICATION);
        }

        // On passe les lignes différentes qu'on a trouvées
        idx_b += nb_lines_different_b;
        idx_a += nb_lines_different_a;
        // Puis on passe la ligne lcs qu'on cherchait
        idx_a++;
        idx_b++;
    }

    /* La boucle précédente s'arrête quand la dernière ligne lcs a été trouvée
    Ici on traite donc les éventuelles modifications faites en fin de fichier, après la dèrnire ligne lcs*/

    // Il en reste dans A et dans B => Modifications
    if (idx_a < size_a && idx_b < size_b)
    {
        nb_lines_different_a = size_a - idx_a;
        nb_lines_different_b = size_b - idx_b;
        if(diff_strcasecmp(file_a[idx_a], file_b[idx_b]) != 0)
            print_edit(file_a, file_b, idx_a, idx_b, nb_lines_different_a, nb_lines_different_b, MODIFICATION);
    }
    // Il en reste dans A mais pas dans B => Suppressions
    else if (idx_a < size_a && idx_b == size_b)
    {
        nb_lines_different_a = size_a - idx_a;
        if(diff_strcasecmp(file_a[idx_a], file_b[idx_b]) != 0)
            print_edit(file_a, file_b, idx_a, idx_b, nb_lines_different_a, 0, DELETION);
    }
    // Il en reste dans B mais pas dans A => Additions
    else if (idx_a == size_a && idx_b < size_b)
    {
        nb_lines_different_b = size_b - idx_b;
        if(diff_strcasecmp(file_a[idx_a], file_b[idx_b]) != 0)
            print_edit(file_a, file_b, idx_a, idx_b, 0, nb_lines_different_b, ADDITION);
    }

}
