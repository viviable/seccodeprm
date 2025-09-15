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
    int len = strlen(input);
    int i;
    for (i = len - 1; i >= 0; i--) {
        if (input[i] != ' ') {
            break;
        }
    }
    if (i < len - 1) {
        input[i + 1] = '\0';
    }

    // get the length of the input
    len = strlen(input);

    // allocate memory for the output
    char *output = malloc(len * sizeof(char));

    // initialize the output
    for (i = 0; i < len; i++) {
        output[i] = input[i];
    }
    output[len] = '\0';

    // go through the string and reverse it
    int j = 0;
    for (i = len - 1; i >= 0; i--) {
        if (input[i] == ' ') {
            // move the space to the beginning
            output[j] = ' ';
            j++;
        } else {
            output[j] = input[i];
            j++;
        }
    }
    output[len] = '\0';

    // print the output
    printf("'%s'\n", output);

    // free the memory
    free(output);

    return 0;
}
