#include <stdio.h>

void kmain()
{
    puts("This is my custom kernel");
    puts("OS-TK has some neat progress!\n");

    int someNumber = 69;
    puts("I can use printf() formating!");
    printf("someNumber = %d\n", someNumber);
}