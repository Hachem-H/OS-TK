#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <Drivers/Display.h>

// Temporary coordinates
void putchar(const char character) { TextRenderer_RenderText(&character, 0, 0);  }
void puts(const char* string)      { TextRenderer_RenderText(string, 0, 0);
                                     TextRenderer_RenderText("\n", 0, 0);        }

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
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case 'c': {
                char output = va_arg(list, int);
                TextRenderer_RenderText(&output, 0, 0);
            } break;
            
            case 'x': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                TextRenderer_RenderText(output, 0, 0);
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
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case 'p': {
                int number = va_arg(list, int);
                char output[24] = {0};
                itoa(number, output, 16);
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case 's': {
                char* output = va_arg(list, char*);
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case '%': {
                TextRenderer_RenderText("%", 0, 0);
            } break;
            }

            continue;
        }
        
        putchar(fmt[i]);
    }

    va_end(list);
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
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case 'c': {
                char output = va_arg(arg, int);
                TextRenderer_RenderText(&output, 0, 0);
            } break;
            
            case 'x': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                TextRenderer_RenderText(output, 0, 0);
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
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case 'p': {
                int number = va_arg(arg, int);
                char output[24] = {0};
                itoa(number, output, 16);
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case 's': {
                char* output = va_arg(arg, char*);
                TextRenderer_RenderText(output, 0, 0);
            } break;

            case '%': {
                TextRenderer_RenderText("%", 0, 0);
            } break;
            }

            continue;
        }
        
        putchar(fmt[i]);
    }
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

