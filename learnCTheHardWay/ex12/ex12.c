#include <stdio.h>

/* This is a comment. */
int main(int argc, char const *argv[])
{
    int i = 0;

    if (argc == 1) {
        printf("You only have one argument. You suck.\n");
    }
    else if (argc > 1 && argc < 4) {
        printf("Here's your arguments:\n");

        for (i = 0; i < argc; i++) {
            printf("%s\n", argv[i]);
        }

        printf("\n");
    }
    else {
        printf("You have too many arguments. You suck.\n");
    }
    
    return 0;
}