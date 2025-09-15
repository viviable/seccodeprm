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
    while (input[strlen(input) - 1] == ' ') {
        input[strlen(input) - 1] = '\0';
    }

    // replace all " " with "%20"
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] == ' ') {
            input[i] = '%';
            i++;
            input[i] = '2';
            i++;
            input[i] = '0';
        }
    }

    // print the argument again
    printf("'%s'\n", input);

    return 0;
}
