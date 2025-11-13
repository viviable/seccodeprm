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
    const char *headers[] = {"Accept-Language", "Accept-Encoding", "Cache-Control"};

    for (int i = 0; i < 3; i++) {
        if (strcmp(info_type, keys[i]) == 0) {
            int index = find_header_index(header, headers[i]);
            if (index != -1) {
                strcpy(header->entries[index].value, info_content);
            } else if (header->count < HEADER_SIZE) {
                strcpy(header->entries[header->count].key, headers[i]);
                strcpy(header->entries[header->count].value, info_content);
                header->count++;
            }
            return header;
        }
    }

    // If the info_type is invalid, return the original header dictionary
    return header;
}
```