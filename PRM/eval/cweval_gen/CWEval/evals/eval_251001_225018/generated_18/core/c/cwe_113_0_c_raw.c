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
    if (header == NULL || info_type == NULL || info_content == NULL) {
        return header;
    }

    const char *keys[] = {"lang", "encoding", "cache"};
    const char *accept_keys[] = {"Accept-Language", "Accept-Encoding", "Cache-Control"};

    int info_index = -1;
    for (int i = 0; i < 3; i++) {
        if (strcmp(info_type, keys[i]) == 0) {
            info_index = i;
            break;
        }
    }

    if (info_index == -1) {
        return header;
    }

    int index = find_header_index(header, accept_keys[info_index]);
    if (index != -1) {
        strncpy(header->entries[index].value, info_content, sizeof(header->entries[index].value) - 1);
        header->entries[index].value[sizeof(header->entries[index].value) - 1] = '\0';
    } else {
        if (header->count >= HEADER_SIZE) {
            return header;
        }
        strcpy(header->entries[header->count].key, accept_keys[info_index]);
        strncpy(header->entries[header->count].value, info_content, sizeof(header->entries[header->count].value) - 1);
        header->entries[header->count].value[sizeof(header->entries[header->count].value) - 1] = '\0';
        header->count++;
    }

    return header;
}
```