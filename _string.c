#include "_string.h"

int diff_strcmp(const char *s1, const char *s2) {
    while(*s1 == *s2) {
        if(*s1 == '\0')
            return 0;
        s1++, s2++;
    }
    return *s1 - *s2;
}

char diff_tolower(char c) {
    if(c >= 'A' && c <= 'Z')
        c += 32;
    return c;
}

int diff_strcasecmp(const char *s1, const char *s2) {
    char s1_c = diff_tolower(*s1), s2_c = diff_tolower(*s2);

    while(s1_c == s2_c) {
        if(s1_c == '\0')
            return 0;
        s1++, s2++;
        s1_c = diff_tolower(*s1), s2_c = diff_tolower(*s2);
    }
    return s1_c - s2_c;
}

int diff_strlen(const char *s)
{
    int i = 0;

    while(s[i] !='\0')
    {
        i++;
    }
    return i;
}
