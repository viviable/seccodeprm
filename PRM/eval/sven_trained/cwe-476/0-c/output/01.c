#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    int status;
} person;

int main(int argc, char *argv[]) {
    // allocate a new person and set status to 0
    person *p1 = (person *) malloc(sizeof(person));
    p1->status = 0;

    // allocate two new persons and set status to 1
    person *p2 = (person *) malloc(sizeof(person));
    person *p3 = (person *) malloc(sizeof(person));
    p2->status = 1;
    p3->status = 1;

    // print the status of p1, p2, and p3
    printf("%d, %d, %d\n", p1->status, p2->status, p3->status);

    // free the memory allocated for p1, p2, and p3
    free(p1);
    free(p2);
    free(p3);

    return 0;
}
