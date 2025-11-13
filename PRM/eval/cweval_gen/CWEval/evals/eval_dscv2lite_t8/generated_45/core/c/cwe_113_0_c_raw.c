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
        return header; // Return original header if no space
    }

    const char *keys[] = {"lang", "encoding", "cache"};
    const char *default_keys[] = {"Accept-Language", "Accept-Encoding", "Cache-Control"};

    int index = -1;
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (strcmp(info_type, keys[i]) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return header; // Return original header if invalid info_type
    }

    strcpy(header->entries[header->count].key, default_keys[index]);
    strcpy(header->entries[header->count].value, info_content);
    header->count++;

    return header;
}
```