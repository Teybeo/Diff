#include "lcs.h"
#include "output.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "options.h"
#include "edit_list.h"
#include "line_length_list.h"

#define ARGV_INPUT true

char** load_file(char* filename, int* nb_lines);
int get_number_of_lines(FILE* file);
Line_Length* get_lines_data(char* text, int* nb);
void print_diff(char** file_a, int size_a, char** file_b, int size_b, char** lcs, int size_lcs);
void is_different(char** file_a, int size_a, char** file_b, int size_b);

int main(int argc, char** argv)
{
    Options* options = NULL;

#ifdef ARGV_INPUT
    if (argc < 3) {
        puts("Nombre de parametres insuffisants");
        return 1;
    }
    char* path_a = argv[1];
    char* path_b = argv[2];

    options = parse_options(argc, argv);

#else
    char path_a[256] = {};
    char path_b[256] = {};
    fgets(path_a, 256, stdin);
    fgets(path_b, 256, stdin);
    path_a[strlen(path_a)-1] = '\0';
    path_b[strlen(path_b)-1] = '\0';
#endif

    if (options == NULL)
    {
        puts("Erreur lecture des options");
        return 0;
    }


    int size_a = 0;
    int size_b = 0;

    char** file_a = load_file(options->path_a, &size_a);
    if (file_a == NULL)
        return 0;

    char** file_b = load_file(options->path_b, &size_b);
    if (file_b == NULL)
        return 0;

    char** lcs = calloc(sizeof(char*), max(size_a, size_b) + 1);

    char** matrix = build_lcs_matrix(file_a, file_b, size_a, size_b);
    extract_lcs(matrix, file_a, file_b, size_a, size_b, lcs);

    putchar('\n');

    int size_lcs = 0;
    while (lcs[size_lcs++] != 0);
    size_lcs--;

    if (options->brief)
        is_different(file_a, size_a, file_b, size_b);
    else if (options->output_mode == OUTPUT_MODE_NORMAL)
    {
        if (options->ignore_case_content)
            ignore_casse(file_a, size_a, file_b, size_b, lcs, size_lcs);
        else
            print_diff_normal(file_a, size_a, file_b, size_b, lcs, size_lcs);
    }

    return 0;
}

FILE* open_file(char* filename) {

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Impossible d'ouvrir '%s'\n", filename);
    }

    return file;
}

char** load_file(char* filename, int* nb_lines) {

    FILE* file = open_file(filename);
    if (file == NULL)
        return NULL;

    puts("---------------------");

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
        printf("%s", text[i]);
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


/*
    Commande - q
    Dit si les fichiers sont diff�rents ou non
*/
void is_different(char** file_a, int size_a, char** file_b, int size_b)
{

    bool found = true;
    int i;

    if(size_a == size_b)
    {
        for (i = 0 ; i < size_a ; i++ )
        {
            found = !strcmp(file_b[i], file_a[i]);
            if (!(found))
                printf("\nLes fichiers sont differents");
        }
    }

    else
        printf("Les fichiers sont differents\n");

}

char* one_space (char* str) // Option b
{
	//Ignoring every multiples spaces in a row and keeping only one
	char* result = malloc ((str_len(str)+1) * sizeof (*result));
		
    int i;
    int j = 0;
    int sp = 0; 

    for (i=0; i < str_len(str); i++)
    {
        if (str[i] == ' ') //If the current character is a space...
        {
            if (sp == 0) //...and is not followed by another space.
            {
                result[j] = str[i];
                sp = 1; 
                j++;
            }
        }
			 
        else //For any other character
        {
            result[j] = str[i];
            sp = 0;
            j++;
        }
    }
    
    result[j] = '\0'; 
	free(str);
    return result;
}

char* tab_to_spaces (char* str) //Option t
{
    int i;
	int j = 0;
    int k = 0;
	int l = 0;

    for (i=0, i < str_len(str[i]); i++)
    {
        if (str[i] == '\t')
        {
            k++;
        }
    }

    char* result = malloc ((str_len(str) + 1 + 3*k) * sizeof (*result));

    for (i=0; i < str_len(str); i++)
    {
        if (str[i] == '\t')
	    {
			for (l=0; l < 4; l++)
			{
				result[j] = ' ';
				j++;
			}
        }
		
        else
        {
            result[j] = str[i];
            j++;
        }
    }
	 
    result[j] = '\0';
  
    return result;
}

char* no_space(char* str) //Option w
{
    char* result = malloc ((str_len(str)+1) * sizeof (*result));

    int i;
    int j = 0;
	 
    for(i = 0; i < str_len(str); i++) 
    {
        if (str[i] != ' ' && str[i] != '\t') //Not a space or a tab
        {
            result[j] = str[i];
            j++;
        }
    }
	  
    result[j] = '\0';
    free(str);
    return result;
}


