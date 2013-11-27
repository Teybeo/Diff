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
    /*  Pour chaque caractère de la chaine a et dans l'ordre
            Si on ne trouve pas ce caractère ou qu'il n'y a plus de caracs dans le lcs
                On marque le caractère de a à cette position comme ayant été supprimé
            Sinon, c'est qu'on a trouvé le carac de a dans le lcs, donc il n'a pas été supprimé, on passe au caractère suivant
        Même chose pour b sauf qu'on marque les caractères des positions non-trouvées comme des additions
    */
    int i, j;

    for (i = j = 0 ; i < size_a ; i++ )
    {
        if (j >= size_lcs || strcmp(file_a[i], lcs[j]) != 0)
        {
            deletion = empilerFin(deletion, file_a[i], i);
        }
        else
            j++;
    }
    for (i = j = 0; i < size_b ; i++ )
    {
        if (j < size_lcs && strcmp(file_b[i], lcs[j]) == 0)
        {
            j++;
        }
        else
            addition = empilerFin(addition, file_b[i], i);
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











