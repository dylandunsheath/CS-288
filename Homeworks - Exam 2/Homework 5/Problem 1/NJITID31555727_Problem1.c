#include <string.h>
#include <ctype.h>
#include <stdio.h>
void printHexPtr(void *);

int main(int argc, char *argv[])
{
    int i;
    char c;
    char *ptr;
    char *last;
    char firstOutput[] = "argv | ";
    printf("%s", firstOutput);;
    printHexPtr(&argv);
    printf(" | %p\n", &argv);
    
    printf("\n");

    // outputs current argc index (ie: argc[0], argc[1], ...)
    // array of pointers
    // command line parameters
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d] | ", i);
        printHexPtr(&argv[i]);

        printf(" | %p\n", &argv[i]);   
    }
    printf("\n");

    ptr = argv[0];

    ptr = (char *)((long)ptr & -8);

    last = argv[argc - 1] + strlen(argv[argc - 1]);

    while (ptr <= last)
    {
        printf(" | ");
        for (i = 7; i >= 0; i--)
        {
            c = ptr[i];
            if (isprint(c))
            {
                // Char as hex and 
                // then the char in text format
                printf("%02hhx(%c) ", c, c);
            }
            
            else
            {
                printf("%02hhx(\\%hhu)", c, c);
            }
            printf(" ");
        }
        printf("| %p\n", ptr);
        ptr += 8;
    }
    printf("\n");
    return 0;
}

void printHexPtr(void *value)
{
    int i;
    char *c;
    c = (char *)value;
    for (i = 7; i >= 0; i--)
    {
        if (i < 7)
        {
            printf(" ");
        }
        printf("%02hhx", c[i]);
    }
}

