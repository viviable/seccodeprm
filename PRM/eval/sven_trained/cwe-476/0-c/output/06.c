#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    int status;
} person;

int main(int argc, char *argv[]) {
    // allocate a new person and set status to 0
    person *p = (person *) malloc(sizeof(person));
    p->status = 0;

    // allocate a new person and set status to 0
    person *q = (person *) malloc(sizeof(person));
    q->status = 0;

    // create a new person and set status to 1
    person r = {"new person", 1};

    // free the memory of p and q
    free(p);
    free(q);

    return 0;
}
