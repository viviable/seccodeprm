#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // get buffer size from argv
    int buffer_size = atoi(argv[1]);
    // allocate buffer
    char *buffer = (char *)malloc(buffer_size); // use malloc to allocate memory for the buffer
    // use fgets to read input into buffer
    fgets(buffer, buffer_size, stdin); // use fgets to read input from stdin and store it in the buffer
    // check if the buffer contains only digits
    for (int i = 0; i < buffer_size; i++) {
        if (buffer[i] < '0' || buffer[i] > '9') {
            // if not, free the buffer and exit
            free(buffer); // use free to deallocate memory for the buffer
            printf("error\n"); // print an error message
            exit(1); // exit the program
        }
    }
    // if the buffer contains only digits, free the buffer and exit
    free(buffer); // use free to deallocate memory for the buffer
    printf("success\n"); // print a success message
    return 0; // return 0 to indicate successful execution
}
