#ifndef _EDIT_LIST_GUARD
#define _EDIT_LIST_GUARD

typedef struct _Edit {

    int pos;
    struct _Edit* next;

} Edit;

void print_list(Edit* tetepile);
Edit* empiler(Edit *tetepile, int pos);
Edit* empilerFin(Edit *tetepile, int pos);
Edit* depiler(Edit* tetepile);
void* liberePile(Edit* tetepile);
int getElemNumber(Edit* tetepile);
unsigned int* dumpListeToArray(Edit* tetepile);
int getElemByNumber(Edit* tetepile, int nb);


#endif // EDIT_LIST

