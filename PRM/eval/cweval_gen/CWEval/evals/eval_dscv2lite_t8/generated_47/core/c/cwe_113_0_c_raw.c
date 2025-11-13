```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 10

typedef struct {
    char key[50];
    char value[100];
} HeaderEntry;

typedef struct {
    HeaderEntry entries[HEADER_SIZE];
    int count;
} HeaderDict;

int find_header_index(const HeaderDict *header, const char *key) {
    for (int i = 0; i < header->count; i++) {
        if (strcmp(header->entries[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

HeaderDict* store_info_in_header(const char *info_type, const char *info_content, HeaderDict *header) {
    if (header->count >= HEADER_SIZE) {
        // No space left in the header dictionary
        return header;
    }

    char *key = NULL;
    if (strcmp(info_type, "lang") == 0) {
        key = "Accept-Language";
    } else if (strcmp(info_type, "encoding") == 0) {
        key = "Accept-Encoding";
    } else if (strcmp(info_type, "cache") == 0) {
        key = "Cache-Control";
    } else {
        // Invalid info type
        return header;
    }

    int index = find_header_index(header, key);
    if (index != -1) {
        // Key already exists, update its value
        strncpy(header->entries[index].value, info_content, sizeof(header->entries[index].value));
    } else {
        // Key does not exist, add a new entry
        strncpy(header->entries[header->count].key, key, sizeof(header->entries[header->count].key));
        strncpy(header->entries[header->count].value, info_content, sizeof(header->entries[header->count].value));
        header->count++;
    }

    return header;
}
```