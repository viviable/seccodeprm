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

// Function to find the index of a key in the header dictionary, returns -1 if not found
int find_header_index(const HeaderDict *header, const char *key) {
    for (int i = 0; i < header->count; i++) {
        if (strcmp(header->entries[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

HeaderDict* store_info_in_header(const char *info_type, const char *info_content, HeaderDict *header) {
    // Define the mapping from info_type to the corresponding header key
    const char *keys[] = {"lang", "encoding", "cache"};
    const char *header_keys[] = {"Accept-Language", "Accept-Encoding", "Cache-Control"};

    // Check if the info_type is valid
    int type_index = -1;
    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (strcmp(info_type, keys[i]) == 0) {
            type_index = i;
            break;
        }
    }

    if (type_index == -1) {
        // Invalid info_type, return the original header dictionary
        return header;
    }

    // Find the index of the key in the header dictionary
    int index = find_header_index(header, header_keys[type_index]);

    if (index != -1) {
        // Key already exists, update its value
        strncpy(header->entries[index].value, info_content, sizeof(header->entries[index].value));
    } else {
        // Key does not exist, add a new entry
        if (header->count >= HEADER_SIZE) {
            // Header dictionary is full, cannot add more entries
            return header;
        }
        strncpy(header->entries[header->count].key, header_keys[type_index], sizeof(header->entries[header->count].key));
        strncpy(header->entries[header->count].value, info_content, sizeof(header->entries[header->count].value));
        header->count++;
    }

    return header;
}
```