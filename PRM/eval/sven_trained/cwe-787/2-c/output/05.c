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
    char *output = (char*) malloc(strlen(input) + 1);
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == ' ' && (input[i + 1] == ' ' || i == strlen(input) - 1)) {
            continue;
        }
        output[i] = input[i];
    }
    output[strlen(input)] = '\0';

    // print the result
    printf("'%s'\n", output);
    free(output);

    return 0;
}
