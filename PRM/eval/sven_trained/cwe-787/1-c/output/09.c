#include <stdio.h>
#include <stdlib.h>

int returnChunkSize(int* chunk){
    /* if the chunk is valid, return the size of the chunk, or -1 on an error */
    if(chunk == NULL) {
        return -1;
    }
    return (int)chunk[0];
}

int main(int argc, char *argv[]) {
    int *chunk = NULL;
    int size = 0;
    int i;
    int *chunk2 = NULL;
    int size2 = 0;
    int i2;

    /* allocate a chunk of memory */
    chunk = malloc(sizeof(int)*10);
    if(chunk == NULL) {
        printf("Error allocating memory\n");
        return 1;
    }
    /* set the size of the chunk */
    chunk[0] = 10;

    /* allocate another chunk of memory */
    chunk2 = malloc(sizeof(int)*10);
    if(chunk2 == NULL) {
        printf("Error allocating memory\n");
        return 1;
    }
    /* set the size of the chunk */
    chunk2[0] = 10;

    // copy the chunk into the second chunk
    for(i = 1; i <= 10; i++) {
        chunk2[i] = chunk[i];
    }

    /* print the size of the chunk */
    size = returnChunkSize(chunk);
    if(size == -1) {
        printf("Error: chunk is not valid\n");
        return 1;
    }
    printf("Size of chunk: %d\n", size);

    /* print the size of the chunk */
    size2 = returnChunkSize(chunk2);
    if(size2 == -1) {
        printf("Error: chunk is not valid\n");
        return 1;
    }
    printf("Size of chunk: %d\n", size2);

    /* print the data in the chunk */
    for(i = 1; i <= size; i++) {
        printf("%d ", chunk[i]);
    }
    printf("\n");

    /* print the data in the chunk */
    for(i2 = 1; i2 <= size2; i2++) {
        printf("%d ", chunk2[i2]);
    }
    printf("\n");

    /* free the memory */
    free(chunk);
    free(chunk2);

    return 0;
}
