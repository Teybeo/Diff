#include "output.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define ADDITION 1
#define DELETION 2
#define MODIFICATION 3

void print_edit(char** file_a, char** file_b, int idx_a, int idx_b, int nb_diff_a, int nb_diff_b, int edit_type);
void print_edit_unified(char** file_a, int size_a, char** file_b, int size_b, int idx_a, int idx_b, int nb_diff_a, int nb_diff_b, int nb_context_lines);
void print_label(char* mark, char* path);
void print_lines(char** file, int start, int count, char symbol);
void print_lines2(char** file, int size, int start, int count, char symbol);
void print_range(int start, int count);
int get_nb_lines_until_found(char** file, char* string, int start, int size, bool ignore_case);

// Rappel: Si un caractère est dans le lcs, alors il est obligatoirement présent dans la chaine de départ ET d'arrivée
/*  Pour chaque élément du lcs et dans l'ordre
        Si on ne trouve pas cet élément lcs dans B mais en le trouvant dans A, c'est que des lignes ont été ajoutées avant dans B
            On indique toutes ces lignes comme des ajouts et on les passe
        Si on ne trouve pas cet élément lcs dans A mais en le trouvant dans B, c'est que des lignes ont été supprimées dans B
            On indique toutes ces lignes comme des suppressions et on les passe
        Si on ne trouve cet élément lcs ni dans A ni dans B, c'est que ces lignes ont été changées de A en celles de B
            On indique toutes ces lignes comme changées et on les passe
*/
void print_diff_normal(char** file_a, int size_a, char** file_b, int size_b, char** lcs, int size_lcs, Options* options) {

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

        if(options->ignore_case_content){
            found_in_a = !strcasecmp(lcs[i], file_a[idx_a]);
            found_in_b = !strcasecmp(lcs[i], file_b[idx_b]);
        }
        else {
            found_in_a = !strcmp(lcs[i], file_a[idx_a]);
            found_in_b = !strcmp(lcs[i], file_b[idx_b]);
        }

        if (!found_in_a || !found_in_b)
        {
            nb_lines_different_a = get_nb_lines_until_found(file_a, lcs[i], idx_a, size_a, options->ignore_case_content);
            nb_lines_different_b = get_nb_lines_until_found(file_b, lcs[i], idx_b, size_b, options->ignore_case_content);

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

void print_diff_unified(char** file_a, int size_a, char** file_b, int size_b, char** lcs, int size_lcs, Options* options) {

    bool found_in_a = true;
    bool found_in_b = true;
    int i, idx_a = 0, idx_b = 0;
    int nb_lines_different_a;
    int nb_lines_different_b;
    int size_hunk_a = 0;
    int size_hunk_b = 0;
    int start_a = 0;
    int start_b = 0;

    print_label("---", options->path_a);
    print_label("+++", options->path_b);

    for (i = 0 ; i < size_lcs ; i++ )
    {
        nb_lines_different_a = 0;
        nb_lines_different_b = 0;

        if(options->ignore_case_content){
            found_in_a = !strcasecmp(lcs[i], file_a[idx_a]);
            found_in_b = !strcasecmp(lcs[i], file_b[idx_b]);
        }
        else {
            found_in_a = !strcmp(lcs[i], file_a[idx_a]);
            found_in_b = !strcmp(lcs[i], file_b[idx_b]);
        }

        if (!found_in_a || !found_in_b)
        {
            nb_lines_different_a = get_nb_lines_until_found(file_a, lcs[i], idx_a, size_a, options->ignore_case_content);
            nb_lines_different_b = get_nb_lines_until_found(file_b, lcs[i], idx_b, size_b, options->ignore_case_content);

            start_a = fmax(1, idx_a+1 - options->nb_context_lines);
            start_b = fmax(1, idx_b+1 - options->nb_context_lines);
            size_hunk_a = nb_lines_different_a + options->nb_context_lines * 2;
            size_hunk_b = nb_lines_different_b + options->nb_context_lines * 2;

//            end = fmaxf(idx_a + nb_lines_different_a, idx_b + nb_lines_different_b) + options->nb_context_lines;
//            start = fminf(idx_a, idx_b) - options->nb_context_lines;

            print_edit_unified(file_a, size_a, file_b, size_b, idx_a, idx_b, nb_lines_different_a, nb_lines_different_b, options->nb_context_lines);
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

    if (idx_a < size_a || idx_b < size_b)
    {
        nb_lines_different_a = size_a - idx_a;
        nb_lines_different_b = size_b - idx_b;

            print_edit_unified(file_a, size_a, file_b, size_b, idx_a, idx_b, nb_lines_different_a, nb_lines_different_b, options->nb_context_lines);
    }

}
void print_label(char* mark, char* path) {

    char buffer[512] = {};
    struct tm *info_time = NULL;
    time_t raw_time = 0;
    time(&raw_time);
    info_time = localtime(&raw_time);
    strftime(buffer, 512, "%Y-%m-%d %H:%M:%S", info_time);

    printf("%s %s\t", mark, path);

    #ifdef _WIN32
        printf("%s.%I64u ", buffer, (unsigned long long)raw_time);
    #else
        printf("%s.%llu ", buffer, (unsigned long long)raw_time);
    #endif

    strftime(buffer, 512, "%z", info_time);
    printf("%s\n", buffer);

}

void print_edit(char** file_a, char** file_b, int idx_a, int idx_b, int nb_diff_a, int nb_diff_b, int edit_type) {

    char edit_code[] = {'a', 'd', 'c'};

    print_range(idx_a, nb_diff_a);
    putchar(edit_code[edit_type-1]);
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

void print_edit_unified(char** file_a, int size_a, char** file_b, int size_b, int idx_a, int idx_b, int nb_diff_a, int nb_diff_b, int nb_context_lines) {

    int nb_context_lines_before = fminf(nb_context_lines, idx_a);
    int nb_context_lines_after = fminf(nb_context_lines, size_a - (idx_a + nb_diff_a) );
    printf("context_lines_before: %d\n", nb_context_lines_before);
    printf("context_lines_after: %d\n", nb_context_lines_after);

    int start_hunk_a = idx_a - nb_context_lines_before;
    int start_hunk_b = idx_b - nb_context_lines_before;
    int end_hunk_a = idx_a + nb_diff_a + nb_context_lines_after;
    int end_hunk_b = idx_b + nb_diff_b + nb_context_lines_after;

    printf("@@ -%d,%d +%d,%d @@\n", start_hunk_a + 1, end_hunk_a - start_hunk_a, start_hunk_b + 1, end_hunk_b - start_hunk_b);

    print_lines2(file_a, size_a, start_hunk_a, nb_context_lines_before, ' ');
    print_lines2(file_a, size_a, idx_a, nb_diff_a, '-');
    print_lines2(file_b, size_b, idx_b, nb_diff_b, '+');
    print_lines2(file_a, size_a, idx_a + nb_diff_a, nb_context_lines_after, ' ');

}

int get_nb_lines_until_found(char** file, char* string, int start, int size, bool ignore_case_content) {

    int i;

    if(ignore_case_content)
        for (i = 0 ; i < size && strcasecmp(file[i + start], string) != 0 ; i++ );
    else
        for (i = 0 ; i < size && strcmp(file[i + start], string) != 0 ; i++ );

    return i;

}
/**
    Affiche les lignes de l'intervalle [start, start + count-1]
**/
void print_lines(char** file, int start, int count, char symbol) {

    int i;
    for (i = 0 ; i < count ; i++ )
        printf("%c %s", symbol, file[start + i]);
}

void print_lines2(char** file, int size, int start, int count, char symbol) {

    int i;
    for (i = start ; i < (start + count) && i < size ; i++ )
        printf("%c %s", symbol, file[i]);
}

void print_range(int start, int count) {

    if (count == 0)
        printf("%d", start); // Ligne après laquelle une modif a été faite
    else if (count == 1)
        printf("%d", start + 1); // Numéro de ligne réel
    else
        printf("%d,%d", start + 1, start + count);
}
