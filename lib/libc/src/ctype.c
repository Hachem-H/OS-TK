#include "ctype.h"
#include "string.h"

int isalnum(int c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= '0' && c <= '9')
        return 1;

    return 0;
}

int isalpha(int c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    if (c >= 'A' && c <= 'Z')
        return 1;

    return 0;
}

int iscntrl(int c)
{
    if (c >= 0x0 && c <= 0x1F)
        return 1;
    if (c == 0x7F)
        return 1;
    return 0;
}

int isdigit(int c)
{
    if (c >= '0' && c <= '9')
        return 1;

    return 0;
}

int isgraph(int c)
{
    if (isalpha(c) || ispunct(c))
        return 1;
    return 0;
}

int islower(int c)
{
    if (c >= 'a' && c <= 'z')
        return 1;
    return 0;
}

int isupper(int c)
{
    if (c >= 'A' && c <= 'Z')
        return 1;
    return 0;
}

int isxdigit(int c)
{
    if (c >= 'A' && c <= 'F')
        return 1;
    if (c >= 'a' && c <= 'f')
        return 1;
    if (c >= '0' && c <= '9')
        return 1;
    return 0;
}


int isprint(int c)
{
    if (isalpha(c) || ispunct(c) || c == ' ')
        return 1;
    return 0;
}

int ispunct(int c)
{
    char symbols[] = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|} ~";
    for (int i =0; i < strlen(symbols); i++)
        if (c == symbols[i])
            return 1;

    return 0;
}

int isspace(int c)
{
    if (c == ' ')
        return 1;
    return 0;
}

int tolower(int c)
{
    if (islower(c))
        return c;
    return c - 'A' + 'a';
}

int toupper(int c)
{
    if (islower(c))
        return c;
    return c - 'a' + 'A';
}
