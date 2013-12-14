#include "options.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Options* parse_options(int argc, char** argv) {

    Options* options = calloc(1, sizeof(Options));

    int i;
    int arg_length = 0;
    char* arg = NULL;
    bool erreur = false;

    for (i = 1 ; i < argc ; i++ )
    {
        arg = argv[i];
        arg_length = strlen(arg);

        printf("%s", arg);

        if (arg_length == 2 && arg[0] == '-' && arg[1] != '-')
        {
            switch (arg[1])
            {
            case 'u':

                options->output_mode = OUTPUT_MODE_UNIFIED;

            break;

            case 'U':

                if (i + 1 == argc)
                {
                    puts("Erreur: pas de nombre spécifié pour l'option -U");
                    erreur = true;
                }
                else
                {
                    options->nb_context_lines = atoi(argv[i + 1]);
                    options->output_mode = OUTPUT_MODE_UNIFIED;
                    i++;
                }

            break;

            case 'q':

                options->brief = true;

            break;

            case 'i':

                options->ignore_case_content = true;

            break;

            default:

                printf("Erreur: [%s] is not an option\n", arg);
                erreur = true;

            break;

            }
        }
        else if (arg_length > 2)
        {
            if (arg[0] == '-' && arg[1] == '-')
            {
                if (strcmp(arg + 2, "normal"))
                    options->output_mode = OUTPUT_MODE_NORMAL;
                if (strcmp(arg + 2, "brief"))
                    options->brief = true;
                if (strcmp(arg + 2, "--ignore-case"))
                    options->ignore_case_content = true;
            }
            else {

                if (strlen(options->path_a) == 0)
                    strcpy(options->path_a, arg);
                else if (strlen(options->path_b) == 0)
                    strcpy(options->path_b, arg);
                else
                {
                    printf("Erreur: [%s] is not an option\n", arg);
                    erreur = true;
                }

            }
        }
        else
        {
            printf("Erreur: [%s] is not an option\n", arg);
            erreur = true;
        }

    }

    if (strlen(options->path_a) == 0 || strlen(options->path_b) == 0)
    {
        puts("Erreur: Il faut 2 fichiers a comparer");
        erreur = true;
    }

    if (erreur)
    {
        free(options);
        return NULL;
    }

    return options;


}
