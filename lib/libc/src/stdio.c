#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <Drivers/Console.h>
#include <Drivers/Keyboard.h>

static FILE stdoutTEMP;
static FILE stderrTEMP;
static FILE stdinTEMP;

FILE* stdout = &stdoutTEMP;
FILE* stderr = &stderrTEMP;
FILE* stdin  = &stdinTEMP; 

void putchar(const char character) { Console_PutChar(character); }
void puts(const char* string)      { Console_PrintString(string);
                                     Console_NewLine();          }

void fputchar(FILE* file, const char character)
{
    if (file == stdout || file == stderr)
        putchar(character);
}

void fputs(FILE* file, const char* string)
{
    if (file == stdout || file == stderr)
        puts(string);
}

void printf(const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);

    for (int i = 0; i < strlen((char*)fmt); i++)
    {
        if (fmt[i] == '%')
        {
            i++;

            switch (fmt[i])
            {
            case 'd': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 10); 
                Console_PrintString(output);
            } break;

            case 'c': {
                char output = va_arg(list, int);
                Console_PutChar(output);
            } break;
            
            case 'x': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                Console_PrintString(output);
            } break;

            case 'X': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int i=0;i<strlen(output);i++)
                    if (output[i] > 'a' && output[i] < 'z')
                    {
                        output[i] -= 'a';
                        output[i] += 'a';
                    }
    
                Console_PrintString(output);
            } break;

            case 'p': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                Console_PrintString(output);
            } break;

            case 's': {
                char* output = va_arg(list, char*);
                Console_PrintString(output);
            } break;

            case '%': {
                Console_PutChar('%');
            } break;
            }

            continue;
        }
        
        putchar(fmt[i]);
    }

    va_end(list);
}

void fprintf(FILE* file, const char* fmt, ...)
{
    if (file == stdout || file == stderr)
        printf(fmt);
}

void vprintf(const char* fmt, va_list arg)
{
    for (int i = 0; i < strlen((char*)fmt); i++)
    {
        if (fmt[i] == '%')
        {
            i++;

            switch (fmt[i])
            {
            case 'd': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 10); 
                Console_PrintString(output);
            } break;

            case 'c': {
                char output = va_arg(arg, int);
                Console_PutChar(output);
            } break;
            
            case 'x': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                Console_PrintString(output);
            } break;

            case 'X': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int i=0;i<strlen(output);i++)
                    if (output[i] > 'a' && output[i] < 'z')
                    {
                        output[i] -= 'a';
                        output[i] += 'a';
                    }
                Console_PrintString(output);
            } break;

            case 'p': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                Console_PrintString(output);
            } break;

            case 's': {
                char* output = va_arg(arg, char*);
                Console_PrintString(output);
            } break;

            case '%': {
                Console_PutChar('%');
            } break;
            }

            continue;
        }
        
        putchar(fmt[i]);
    }
}

void vfprintf(FILE* file, const char* fmt, va_list arg)
{
    if (file == stdout || file == stderr)
        vprintf(fmt, arg);
}

void sprintf(char* string, const char* fmt, ...)
{
    int j = 0;
    va_list list;
    va_start(list, fmt);

    for (int i = 0; i < strlen((char*)fmt); i++)
    {

        if (fmt[i] == '%')
        {
            i++;

            switch (fmt[i])
            {
            case 'd': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 10); 
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 'c': {
                char output = va_arg(list, int);
                string[j] = output;
            } break;
            
            case 'x': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 'X': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int i=0;i<strlen(output);i++)
                    if (output[i] > 'a' && output[i] < 'z')
                    {
                        output[i] -= 'a';
                        output[i] += 'a';
                    }

                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 'p': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 's': {
                char* output = va_arg(list, char*);
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case '%': {
                string[j] = fmt[i];
            } break;
            }

            continue;
        }
        
        string[j] = fmt[i];
        j++;
    }

    va_end(list);
}

void vsprintf(char* string, const char* fmt, va_list arg)
{
    int j = 0;

    for (int i = 0; i < strlen((char*)fmt); i++)
    {

        if (fmt[i] == '%')
        {
            i++;

            switch (fmt[i])
            {
            case 'd': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 10); 
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 'c': {
                char output = va_arg(arg, int);
                string[j] = output;
            } break;
            
            case 'x': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 'X': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int i=0;i<strlen(output);i++)
                    if (output[i] > 'a' && output[i] < 'z')
                    {
                        output[i] -= 'a';
                        output[i] += 'a';
                    }

                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 'p': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case 's': {
                char* output = va_arg(arg, char*);
                for (int x=0; x<strlen(output);x++)
                    string[j++] = output[x];
            } break;

            case '%': {
                string[j] = fmt[i];
            } break;
            }

            continue;
        }
        
        string[j] = fmt[i];
        j++;
    }
}

// --------------------------------- //

char fgetchar()
{
    return KeyboardReadChar();
}