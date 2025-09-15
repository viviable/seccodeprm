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
    // print status
    printf("%d\n", p->status);
    // allocate a new person and set status to 1
    p = (person *)realloc(p, sizeof(person) * 2);
    p[1].status = 1;
    // print status
    printf("%d\n", p[1].status);
    return 0;
}
