#ifndef _LINE_LENGTH_LIST_GUARD
#define _LINE_LENGTH_LIST_GUARD

typedef struct _Line_Length {

    int length;
    char* data;
    struct _Line_Length* next;

} Line_Length;

void print_LL_list(Line_Length* tetepile);
Line_Length* empiler_LL(Line_Length *tetepile, int length, char* data);
Line_Length* empilerFin_LL(Line_Length *tetepile, int length, char* data);
Line_Length* depiler_LL(Line_Length* tetepile);
void* liberePile_LL(Line_Length* tetepile);
//int getElemNumber(Line_Length* tetepile);
//unsigned int* dumpListeToArray(Line_Length* tetepile);


#endif // LINE_LENGTH_LIST

