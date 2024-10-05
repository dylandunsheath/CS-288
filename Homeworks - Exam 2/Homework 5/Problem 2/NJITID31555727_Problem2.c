#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int option;
    // MUST put ':' in the start of the string so program can distinguish between '?' and ':' 
    while ((option=getopt(argc,argv,":cduf:s:")) != -1) {
        switch (option) {
            case 'c':
               
            case 'd':
                
            case 'u':
                printf("option: %c\n", option);
                break;
            case 'f':
                printf("Extra argument for %c: %s\n", option, optarg);
                break;            
            case 's':
                printf("Extra argument for %c: %s\n", option, optarg);
                break;
            case ':':
                printf("option %c needs a value\n", optopt);
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
        }
    }
    for(; optind < argc; optind++)
        printf("Other arguments: %s\n", argv[optind]);

    exit(0);
}

