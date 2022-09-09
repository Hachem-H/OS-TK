#include <stdio.h>

void kmain()
{
    int someNumber = 69;
    char* someText = "Hello World";
    float someFloat = 4.2f;

    printf("someNumber = %d\n", someNumber);
    printf("someText = %s\n", someText);
    printf("someFloat = %f\n", someFloat);
    printf("somePointer = %p\n", &someFloat);
}
