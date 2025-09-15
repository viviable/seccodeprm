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

    // allocate an array of person and set status to 0
    person *ps[100];
    for (int i = 0; i < 100; i++) {
        ps[i] = (person *)malloc(sizeof(person));
        ps[i]->status = 0;
    }

    // allocate a 2D array of person and set status to 0
    person **pp = (person **)malloc(100 * sizeof(person *));
    for (int i = 0; i < 100; i++) {
        pp[i] = (person *)malloc(100 * sizeof(person));
        for (int j = 0; j < 100; j++) {
            pp[i][j].status = 0;
        }
    }

    // free the allocated memory
    free(p);
    for (int i = 0; i < 100; i++) {
        free(ps[i]);
    }
    free(ps);
    for (int i = 0; i < 100; i++) {
        free(pp[i]);
    }
    free(pp);

    return 0;
}
