#include "edit_list.h"

#include <stdlib.h>
#include <stdio.h>

Edit* empiler(Edit *tetepile, char data, int pos) {

    Edit *nouveau = malloc(sizeof(Edit));
    nouveau->data = data;
    nouveau->pos = pos;
    nouveau->next = tetepile;

    return nouveau;

}

Edit* empilerFin(Edit *tetepile, char data, int pos) {

    Edit *nouveau = malloc(sizeof(Edit));
    nouveau->data = data;
    nouveau->pos = pos;
    nouveau->next = NULL;

    if (tetepile == NULL)
        tetepile = nouveau;
    else {

        Edit* curseur = tetepile;
        while (curseur->next != NULL) {

            curseur = curseur->next;
        }
        curseur->next = nouveau;
    }

    return tetepile;

}

Edit* depiler(Edit* tetepile) {

    if (tetepile == NULL)
        return NULL;

    else {

        Edit* restant = tetepile->next;
        free(tetepile);
        return restant;
    }
}

void* liberePile(Edit* tetepile) {

    while (tetepile != NULL) {

        tetepile = depiler(tetepile);
    }

    return NULL;

}

int getElemNumber(Edit* tetepile) {

    Edit* curseur = tetepile;
    int nb = 0;

    while (curseur != NULL)
    {
        nb++;
        curseur = curseur->next;
    }

    return nb;

}

// On extrait chaque x des vecteurs de la liste tetepile et
// on les range dans une case du tableau
// L'ordre est inversé pour que le bas de la pile soit au début du tableau
unsigned int* dumpListeToArray(Edit* tetepile) {

    Edit* curseur = tetepile;
    int nb = getElemNumber(tetepile);
    unsigned int* array = malloc(sizeof(unsigned int)*nb);

    int i = 0;
    while (curseur != NULL)
    {
        array[nb-1 - i] = curseur->pos;
        curseur = curseur->next;
        i++;
    }

    return array;

}



int getElemByNumber(Edit* tetepile, int nb) {

    Edit* curseur = tetepile;

    int step = 0;
    while (curseur != NULL && step <= nb-1)
    {
        step++;
        curseur = curseur->next;
    }

    return curseur->pos;

}

void print_list(Edit* tetepile) {

    while (tetepile != NULL) {

        printf("%d: %c ", tetepile->pos, tetepile->data);
        tetepile = tetepile->next;
    }
    putchar('\n');
}
