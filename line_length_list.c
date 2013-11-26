#include "line_length_list.h"

#include <stdlib.h>
#include <stdio.h>

void print_list_LL(Line_Length* tetepile) {

    while (tetepile != NULL) {
        printf("length: %d\n", tetepile->length);
        tetepile = tetepile->next;
    }
}

Line_Length* empiler_LL(Line_Length *tetepile, int length, char* data) {

    Line_Length *nouveau = malloc(sizeof(Line_Length));
    nouveau->length = length;
    nouveau->data = data;
    nouveau->next = tetepile;

    return nouveau;
}

Line_Length* empilerFin_LL(Line_Length *tetepile, int length, char* data) {

    Line_Length *nouveau = malloc(sizeof(Line_Length));
    nouveau->length = length;
    nouveau->data = data;
    nouveau->next = NULL;

    if (tetepile == NULL)
        tetepile = nouveau;
    else {

        Line_Length* curseur = tetepile;
        while (curseur->next != NULL) {

            curseur = curseur->next;
        }
        curseur->next = nouveau;
    }

    return tetepile;
}

Line_Length* depiler_LL(Line_Length* tetepile) {

    if (tetepile == NULL)
        return NULL;

    else {

        Line_Length* restant = tetepile->next;
        free(tetepile);
        return restant;
    }

}

void* liberePile_LL(Line_Length* tetepile) {

    while (tetepile != NULL) {

        tetepile = depiler_LL(tetepile);
    }

    return NULL;
}

//int getElemNumber(Line_Length* tetepile);
//unsigned int* dumpListeToArray(Line_Length* tetepile);

