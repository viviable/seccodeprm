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
    const char *keys[] = {"lang", "encoding", "cache"};
    const char *default_keys[] = {"Accept-Language", "Accept-Encoding", "Cache-Control"};

    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (strcmp(info_type, keys[i]) == 0) {
            int index = find_header_index(header, default_keys[i]);
            if (index != -1) {
                strncpy(header->entries[index].value, info_content, sizeof(header->entries[index].value) - 1);
            } else if (header->count < HEADER_SIZE) {
                strncpy(header->entries[header->count].key, default_keys[i], sizeof(header->entries[header->count].key) - 1);
                strncpy(header->entries[header->count].value, info_content, sizeof(header->entries[header->count].value) - 1);
                header->count++;
            }
            return header;
        }
    }

    // If info_type is invalid, return the original header dictionary
    return header;
}
```