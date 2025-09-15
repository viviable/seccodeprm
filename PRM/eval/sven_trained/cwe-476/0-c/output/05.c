#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    int status;
} person;

int main(int argc, char *argv[]) {
    // allocate a new person and set status to 0
    person *p = malloc(sizeof(person));
    p->status = 0;

    // print out the memory address of the person and the value of status
    printf("person = %p, status = %d\n", (void *)p, p->status);

    // free the memory and print out the memory address
    free(p);
    printf("person = %p\n", (void *)p);

    return 0;
}
