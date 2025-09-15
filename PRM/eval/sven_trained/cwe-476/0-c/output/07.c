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

    // allocate an array of 5 persons and set all statuses to 0
    person *ps[5];
    for (int i = 0; i < 5; i++) {
        ps[i] = malloc(sizeof(person));
        ps[i]->status = 0;
    }

    // free the allocated memory
    free(p);
    for (int i = 0; i < 5; i++) {
        free(ps[i]);
    }

    // try to access a freed memory
    printf("name: %s\n", p->name); // this will cause a segmentation fault

    return 0;
}
