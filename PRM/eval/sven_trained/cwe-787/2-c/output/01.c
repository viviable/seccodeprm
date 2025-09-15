#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // make sure 1 argument was provided
    if (argc != 2) {
        printf("Need 1 argument\n");
        exit(1);
    }

    // get the argument
    char *input = argv[1];
    
    // print the argument
    printf("'%s'\n", input);

    // remove trailing whitespaces of the argument
    int i;
    int len = strlen(input);
    for (i = len - 1; i >= 0; i--) {
        if (input[i] == ' ' || input[i] == '\t') {
            input[i] = 0;
        } else {
            break;
        }
    }

    // check if it's a palindrome
    int j;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        if (input[i] != input[j]) {
            printf("Not a palindrome\n");
            exit(0);
        }
    }

    printf("Is a palindrome\n");
    exit(0);
}
