void kmain()
{
    int* test = (int*)0x80000000000;
    *test = 2;
    for(;;);
}