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

    // printf to see the value of status
    printf("%d\n", p->status);

    // change the status to 1
    p->status = 1;

    // printf to see the value of status
    printf("%d\n", p->status);

    return 0;
}
