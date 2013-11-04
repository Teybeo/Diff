#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b)((a) > (b) ? (a) : (b))

int size_a = -1;
int size_b = -1;

int get_size_of_lcs(char* a, char* b, int i, int j);
void display_matrix(char** matrix, int n, int m);
char** build_path(char* a, char* b);
void print_LCS(char** matrix, char* a, char* b, int i, int j);

int main(int argc, char** argv)
{
    if (argc < 3) {
        puts("Nombre de parametres insuffisants");
        return 1;
    }

    size_a = strlen(argv[1]);
    size_b = strlen(argv[2]);

//    int size_lcs = get_size_of_lcs(argv[1], argv[2], 0, 0);

//    printf("%d\n", size_lcs);
    char** matrix = build_path(argv[1], argv[2]);
    display_matrix(matrix, size_a+2, size_b+2);
    print_LCS(matrix, argv[1], argv[2], size_a, size_b);

    return 0;
}

char** build_path(char* a, char* b) {

    char** matrix = malloc(sizeof(char*) * (size_a+2));
    int i;
    for (i = 0 ; i < size_a+2 ; i++ )
    {
        matrix[i] = malloc(sizeof(char) * (size_b + 2));
        memset(matrix[i], '0', sizeof(char) * (size_b + 2));
    }

    strcpy(matrix[0]+2, b);
    for (i = 2 ; i < size_a +2; i++ )
        matrix[i][0] = a[i-2];


    int j;
    for (i = 2 ; i < size_a + 2 ; i++ )
    {
        for (j = 2 ; j < size_b + 2; j++ )
        {
            if (a[i-2] == b[j-2])
                matrix[i][j] = 1 + matrix[i-1][j-1];
            else
                matrix[i][j] = max(matrix[i-1][j], matrix[i][j-1]);
            display_matrix(matrix, size_a+2, size_b+2);
        }
    }

    return matrix;
}

void print_LCS(char** matrix, char* a, char* b, int i, int j)
{
    if (i == 0 || j == 0)
        return;

    if (a[i - 1] == b[j - 1])
    {
        print_LCS(matrix, a, b, i - 1, j - 1);
        printf("%c", a[i - 1]);
    }
    else if (matrix[i][j] == matrix[i - 1][j])
        print_LCS(matrix, a, b, i - 1, j);
    else
        print_LCS(matrix, a, b, i, j - 1);
}

int get_size_of_lcs(char* a, char* b, int i, int j) {

    if (i >= size_a || j >= size_b)
        return 0;

    if (a[i] == b[j])
        return 1 + get_size_of_lcs(a, b, i+1, j+1);

    else
        return max(get_size_of_lcs(a, b, i+1, j), get_size_of_lcs(a, b, i, j+1));

}

FILE* open_file(char* filename) {

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Impossible d'ouvrir '%s'\n", filename);
    }

    return file;
}

void display_matrix(char** matrix, int n, int m) {

    int i, j;
    for (i = 0 ; i < n ; i++ )
    {
        for (j = 0 ; j < m; j++ )
            printf("%c ", matrix[i][j]);

        puts("");
    }
        puts("");
}
