#include "lcs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit_list.h"
#include "line_length_list.h"

#define ARGV_INPUT true

char** load_file(char* filename, int* nb_lines);
int get_number_of_lines(FILE* file);
Line_Length* get_lines_data(char* text, int* nb);

int main(int argc, char** argv)
{

#ifdef ARGV_INPUT
    if (argc < 3) {
        puts("Nombre de parametres insuffisants");
        return 1;
    }
    char* path_a = argv[1];
    char* path_b = argv[2];
#else
    char path_a[256] = {};
    char path_b[256] = {};
    fgets(path_a, 256, stdin);
    fgets(path_b, 256, stdin);
    path_a[strlen(path_a)-1] = '\0';
    path_b[strlen(path_b)-1] = '\0';
#endif

    int size_a = 0;
    int size_b = 0;

    char** file_a = load_file(path_a, &size_a);
    if (file_a == NULL)
        return 0;

    char** file_b = load_file(path_b, &size_b);
    if (file_b == NULL)
        return 0;

    char** lcs = calloc(sizeof(char*), max(size_a, size_b) + 1);

//    int size_lcs = get_size_of_lcs(file_a, file_b, 0, 0);
//    printf("%d\n", size_lcs);

    char** matrix = build_lcs_matrix(file_a, file_b, size_a, size_b);
    extract_lcs(matrix, file_a, file_b, size_a, size_b, lcs);

//    puts(lcs);

    Edit* addition = NULL;
    Edit* deletion = NULL;

//    int size_lcs = max(size_a, size_b);
    int size_lcs = 0;
    while (lcs[size_lcs++] != 0);
    size_lcs--;

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
            if (strcmp(file_a[j], lcs[i]) == 0)
            {
                j++;
                break;
            }
            else
                deletion = empilerFin(deletion, file_a[j], j);
        }

        for ( ; k < size_b ; k++ )
        {
            if (strcmp(file_b[k], lcs[i]) == 0)
            {
                k++;
                break;
            }
            else
                addition = empilerFin(addition, file_b[k], k);
        }
    }

    if (size_lcs < size_a) {
        for ( ; j < size_a; j++)
            deletion = empilerFin(deletion, file_a[j], j);
    }
    if (size_lcs < size_b) {
        for ( ; k < size_b; k++)
            addition = empilerFin(addition, file_b[k], k);
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

char** load_file(char* filename, int* nb_lines) {

    FILE* file = open_file(filename);
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char* buffer = malloc(sizeof(char) * size);

    fread(buffer, sizeof(char), size, file);
    rewind(file);

    Line_Length* liste = get_lines_data(buffer, nb_lines);
    printf("nb_lines: %d\n", *nb_lines);

    Line_Length* curseur = liste;

    char** text = malloc(sizeof(char*) * *nb_lines);
    int i;
    for (i = 0 ; i < *nb_lines ; i++ )
    {
        text[i] = calloc(sizeof(char), curseur->length + 1);
        strncpy(text[i], curseur->data, curseur->length);
        curseur = curseur->next;
    }

    return text;

}

int get_number_of_lines(FILE* file) {

    if (file == NULL)
        return 0;

    int nb_lines = 0;
    int i = 0;
    char c;

    while ( (c = fgetc(file)) != EOF && c != '\0')
    {
        if (c == '\n')
            nb_lines++;
    }

    return nb_lines;
}


Line_Length* get_lines_data(char* text, int* nb) {

    if (text == NULL)
        return 0;

    Line_Length* liste = NULL;

    int nb_lines = 0;
    int length = 0;
    int i = 0;
    char* data = text;

    while (text[i] != EOF && text[i] != '\0')
    {
        length++;
        if (text[i] == '\n')
        {
            (*nb)++;
            liste = empilerFin_LL(liste, length, data);
            length = 0;
            data = text+i+1;
        }
//        nb += (text[i] == '\n');
        i++;
    }

    return liste;
}











