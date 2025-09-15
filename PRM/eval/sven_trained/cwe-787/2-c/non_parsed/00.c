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
    int i = len-1;
    while (i >= 0 && input[i] == ' ') {
        i--;
    }
    input[i+1] = '\0';
    len = strlen(input);

    // print the argument without trailing whitespaces
    printf("'%s'\n", input);

    // count the words in the argument
    int numWords = 1;
    int i = 0;
    while (i < len) {
        if (input[i] == ' ') {
            numWords++;
        }
        i++;
    }

    // print the number of words in the argument
    printf("%d\n", numWords);

    // count the letters in the argument
    int numLetters = 0;
    i = 0;
    while (i < len) {
        if (input[i] >= 'a' && input[i] <= 'z') {
            numLetters++;
        }
        i++;
    }

    // print the number of letters in the argument
    printf("%d\n", numLetters);
}
