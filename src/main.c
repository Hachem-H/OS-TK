#include <stdio.h>

void kmain()
{
    printf("Hello World!\n");
    
    char character = 0;
    while (character != '\n')
        character = fgetchar();

    printf("DONE");
}