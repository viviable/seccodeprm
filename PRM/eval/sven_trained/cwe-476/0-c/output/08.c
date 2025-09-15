#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[100];
    int status;
} person;

int main(int argc, char *argv[]) {
    // allocate a new person and set status to 0
    person *p = (person *)malloc(sizeof(person));
    p->status = 0;

    // print p
    printf("p: %p\n", p);

    // allocate a new person and set status to 1
    person *p1 = (person *)malloc(sizeof(person));
    p1->status = 1;

    // print p1
    printf("p1: %p\n", p1);

    // allocate a new person and set status to 2
    person *p2 = (person *)malloc(sizeof(person));
    p2->status = 2;

    // print p2
    printf("p2: %p\n", p2);

    // free the memory for p
    free(p);

    // print p
    printf("p: %p\n", p);

    // free the memory for p1
    free(p1);

    // print p1
    printf("p1: %p\n", p1);

    // free the memory for p2
    free(p2);

    // print p2
    printf("p2: %p\n", p2);

    // return 0
    return 0;
}
