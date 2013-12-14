#ifndef _OPTIONS_GUARD
#define _OPTIONS_GUARD

#include <stdbool.h>

enum Output_Mode {

    OUTPUT_MODE_NORMAL,
    OUTPUT_MODE_UNIFIED,
};

typedef struct _Options {

    char path_a[256];
    char path_b[256];
    enum Output_Mode output_mode;
    bool brief;
    bool ignore_case_content;
    int nb_context_lines;

} Options;

Options* parse_options(int argc, char** argv);

#endif // OPTIONS

