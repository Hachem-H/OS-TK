#include "stdlib.h"
#include "stddef.h"
#include "string.h"
#include "ctype.h"
#include "stdio.h"

#include <Drivers/Display.h>
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

void putchar_(char character) { character; }

void fOutputStream(char character, void* stream)
{
    FILE* file = (FILE*)stream;
    fputc(file, character);
}

void putc(const char character)
{
    putchar(character);
}

void fputc(FILE* file, const char character)
{
    if (file == stdout || file == stderr)
        putchar(character);
}

void fputs(FILE* file, const char* string)
{
    if (file == stdout || file == stderr)
        puts(string);
}

int printf(const char* fmt, ...)
{
    return fprintf(stdout, fmt);
}

int vprintf(const char* fmt, va_list arg)
{
    return vfprintf(stdout, fmt, arg);
}

int fprintf(FILE* file, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const int ret = vfprintf(file, fmt, args);
    va_end(args);
    return ret;
}

int snprintf(char* string, size_t size, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const int ret = vsnprintf(string, size, fmt, args);
    va_end(args);
    return ret;
}

int vfprintf(FILE* file, const char* fmt, va_list arg)
{
    return 0;
}

int vsnprintf(char* string, size_t size, const char* fmt, va_list arg)
{
    return 0;
}

char getc(FILE* file)
{
    if (file == stdin)
        return KeyboardReadChar();
    return EOF;
}

char getchar()
{
    return getc(stdin);
}

void fgets(char* string, size_t size, FILE* stream)
{
    if (stream != stdin)
        return;
    
    size_t index = 0;
    char buffer = 0;
    while (buffer != '\n')
    {
        buffer = getc(stream);

        if (buffer == '\b')
            if (index > 0)
            {
                string[index] = 0;
                index--;
            }
        
        if (index != size)
        {
            string[index] = buffer;
            index++;
        }
    }
    
    if (index != size)
        string[index] = 0;
}

size_t fread(void* pointer, size_t size, size_t nmemb, FILE* stream)
{
    register char* charPointer = (char*) pointer;
    register int character;
    register size_t sizeTemp;
    size_t numberDone = 0;

    if (size)
        while (numberDone < nmemb)
        {
            sizeTemp = size;
            do
            {
                if ((character = getc(stream)) != EOF)
                    *charPointer++ = character;
                else
                    return numberDone;
            } while (--sizeTemp);
            numberDone++;
        }

    return numberDone;
}

int scanf(const char* fmt, ...)
{
    return fscanf(stdin, fmt);
}

int fscanf(FILE* file, const char* fmt, ...)
{
    if (file == stdin)
    {
        size_t size = TextRenderer_GetHeight()*TextRenderer_GetWidth();
        char* buffer = (char*) malloc(size);
        fgets(buffer, size, stdin);
        size_t ret = sscanf(buffer, fmt);
        free(buffer);
        return ret;
    }

    return EOF;
}

int sscanf(char* buffer, const char* format, ...) 
{
    #define CONSUME_SPACE()  while (isspace(*bufferPointer)) { bufferPointer++; }
    #define CHECK(x)         if (!(x)) goto exit;
    #define MATCH()          CHECK(*bufferPointer == *formatPointer);
    #define CHECK_NULL(ptr)  CHECK(NULL != ptr);
    #define CHECK_STRING()   CHECK(0 != maxWidth);
    #define CHECK_BUFFER()   CHECK('\0' != *buffer);
    #define CHECK_STRTONUM() CHECK(bufferPointer != endPointer);

    int argsSet = 0;

    const char* bufferPointer = buffer;
    const char* formatPointer = format;

    unsigned long*   ulongPointer;
    long*             longPointer;
    unsigned int*     uintPointer;
    int*               intPointer;
    unsigned short* ushortPointer;
    short*           shortPointer;
    double*         doublePointer;
    float*           floatPointer;
    char*             charPointer;

    char* endPointer;
    int base;

    int suppressed = 0;
    size_t maxWidth = 0;
    char stringEnding = '\0';

    if (buffer == NULL || format == NULL)
        return -1;

    va_list args;
    va_start(args, format);
    
    while (*formatPointer != '\0') 
    {
        if (isspace(*formatPointer)) 
        {
            CONSUME_SPACE();
            formatPointer++;
            continue;
        }

        if ('%' == *formatPointer) 
        {
            formatPointer++;

            if ('*' == *formatPointer) 
            {
                suppressed = 1;
                formatPointer++;
            } else
                suppressed = 0;

            if (isdigit( *formatPointer)) 
            {
                maxWidth = strtoul(formatPointer, &endPointer, 0);
                CHECK(formatPointer != endPointer);
                CHECK(maxWidth > 0);
                formatPointer = endPointer;
            }

            if ('h' == *formatPointer || 'l' == *formatPointer)
            {
                stringEnding = *formatPointer;
                formatPointer++;
            } else
                stringEnding = '\0';

            if ('n' == *formatPointer) 
            {
                CHECK(!suppressed);
                if ('l' == stringEnding) 
                {
                    longPointer = va_arg(args, long*);
                    CHECK_NULL(longPointer);
                    *longPointer = (long)(bufferPointer - buffer);
                } else if ('h' == stringEnding) 
                {
                    shortPointer = va_arg(args, short*);
                    CHECK_NULL(shortPointer);
                    *shortPointer = (short)(bufferPointer - buffer);
                } else 
                {
                    intPointer = va_arg(args, int*);
                    CHECK_NULL(intPointer);
                    *intPointer = (int)(bufferPointer - buffer);
                }

                formatPointer++;
                argsSet++;
                continue;
            }

            CHECK_BUFFER();

            if ('%' == *formatPointer) 
            {
                CONSUME_SPACE();
                MATCH();
                bufferPointer++;
            } else if ('c' == *formatPointer || 's' == *formatPointer) 
            {
                CHECK_STRING();

                if ('c' != *formatPointer)
                    CONSUME_SPACE();

                if (suppressed) 
                {
                    for (; maxWidth > 0; maxWidth--) 
                    {
                        bufferPointer++;
                        if ( *bufferPointer == '\0' || 
                            (isspace( *bufferPointer) &&
                             's' == *formatPointer))
                            break;
                    }

                    formatPointer++;
                    continue;
                } else {
                    charPointer = va_arg(args, char*);
                    CHECK_NULL(charPointer);

                    for (; maxWidth > 0; maxWidth--) 
                    {
                        *charPointer = *bufferPointer;
                        if ( *bufferPointer == '\0' || 
                            (isspace( *bufferPointer) &&
                             's' == *formatPointer))
                            break;
                        charPointer++;
                        bufferPointer++;
                    }

                    if ('s' == *formatPointer)
                        *charPointer = '\0';
                    argsSet++;
                }

            } else if ('[' == *formatPointer) 
            {
                CHECK(0);
                CHECK_STRING();
            } else if ('i' == *formatPointer || 'd' == *formatPointer) 
            {
                CONSUME_SPACE();
                base = ('d' == *formatPointer) * 10;

                if (suppressed)
                    strtol(bufferPointer, &endPointer, base);
                else if ('l' == stringEnding) 
                {
                    longPointer = va_arg(args, long*);
                    CHECK_NULL(longPointer);
                    *longPointer = (long) strtol(bufferPointer, &endPointer, base);
                }
                else if ('h' == stringEnding) 
                {
                    shortPointer = va_arg(args, short*);
                    CHECK_NULL(shortPointer);
                    *shortPointer = (short)(strtol(bufferPointer, &endPointer, base));
                }
                else 
                {
                    intPointer = va_arg(args, int*);
                    CHECK_NULL(intPointer);
                    *intPointer = (int)(strtol(bufferPointer, &endPointer, base));
                }

                CHECK_STRTONUM();
                bufferPointer = endPointer;
                argsSet++;

            } 
            else if ('g' == *formatPointer || 'e' == *formatPointer || 'f' == *formatPointer ||
                     'G' == *formatPointer || 'E' == *formatPointer || 'F' == *formatPointer)
            {
                CONSUME_SPACE();

                if (suppressed)
                    strtod(bufferPointer, &endPointer);
                else if ('l' == stringEnding) 
                {
                    doublePointer = va_arg(args, double*);
                    CHECK_NULL(doublePointer);
                    *doublePointer = (double)(strtod(bufferPointer, &endPointer));
                } 
                else 
                {
                    floatPointer = va_arg(args, float*);
                    CHECK_NULL(floatPointer);
                    *floatPointer = (float)(strtod(bufferPointer, &endPointer));
                }

                CHECK_STRTONUM();
                bufferPointer = endPointer;
                argsSet++;
            } 
            else if ('u' == *formatPointer || 'o' == *formatPointer || 'x' == *formatPointer || 'X' == *formatPointer) {
                CONSUME_SPACE();
                base = ('u' == *formatPointer) *10 + ('o' == *formatPointer) *8 +
                       ('x' == *formatPointer || 'X' == *formatPointer) *16;

                if (suppressed)
                    strtoul(bufferPointer, &endPointer, base);
                else if ('l' == stringEnding) 
                {
                    ulongPointer = va_arg(args, unsigned long*);
                    CHECK_NULL(ulongPointer);
                    *ulongPointer = (unsigned long) strtoul(bufferPointer, &endPointer, base);
                } 
                else if ('h' == stringEnding) 
                {
                    ushortPointer = va_arg(args, unsigned short*);
                    CHECK_NULL(ushortPointer);
                    *ushortPointer = (unsigned short)(strtoul(bufferPointer, &endPointer, base));
                } 
                else 
                {
                    uintPointer = va_arg(args, unsigned int*);
                    CHECK_NULL(uintPointer);
                    *uintPointer = (unsigned int)(strtoul(bufferPointer, &endPointer, base));
                }

                CHECK_STRTONUM();
                bufferPointer = endPointer;
                argsSet++;

            } else
                CHECK(0);
        } else 
        {
            MATCH();
            bufferPointer++;
        }

        formatPointer++;
    }

    exit:
        va_end(args);
    return argsSet;
}
