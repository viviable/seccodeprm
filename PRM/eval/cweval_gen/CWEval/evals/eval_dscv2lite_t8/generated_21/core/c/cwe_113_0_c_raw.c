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
    // Define the mapping of info_type to header keys
    const char *keys[] = {"lang", "encoding", "cache"};
    const char *headers[] = {"Accept-Language", "Accept-Encoding", "Cache-Control"};

    // Check if the info_type is valid
    int type_index = -1;
    for (int i = 0; i < 3; i++) {
        if (strcmp(info_type, keys[i]) == 0) {
            type_index = i;
            break;
        }
    }

    if (type_index == -1) {
        return header; // Return the original header if info_type is invalid
    }

    // Find the index of the existing entry for the same header key
    int entry_index = find_header_index(header, headers[type_index]);

    // If the entry exists, update its value
    if (entry_index != -1) {
        strncpy(header->entries[entry_index].value, info_content, sizeof(header->entries[entry_index].value) - 1);
        header->entries[entry_index].value[sizeof(header->entries[entry_index].value) - 1] = '\0';
    } else {
        // If the entry does not exist, add a new one
        if (header->count >= HEADER_SIZE) {
            return header; // Return the original header if no space is available
        }
        strncpy(header->entries[header->count].key, headers[type_index], sizeof(header->entries[header->count].key) - 1);
        header->entries[header->count].key[sizeof(header->entries[header->count].key) - 1] = '\0';
        strncpy(header->entries[header->count].value, info_content, sizeof(header->entries[header->count].value) - 1);
        header->entries[header->count].value[sizeof(header->entries[header->count].value) - 1] = '\0';
        header->count++;
    }

    return header;
}
```