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
    
    // print the address of the person
    printf("Address of the person: %p\n", p);
    
    // set the name of the person to "Alice"
    strcpy(p->name, "Alice");
    
    // print the name and status of the person
    printf("Name: %s\n", p->name);
    printf("Status: %d\n", p->status);
    
    // free the allocated memory
    free(p);
    
    return 0;
}
