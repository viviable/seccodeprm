#include <archive_entry.h>
#include <jwt.h>
#include <errno.h>
#include <sys/wait.h>
#include <libxml/xpath.h>
#include <sqlite3.h>
#include <archive.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <regex.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <archive.h>
#include <archive_entry.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <jwt.h>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <regex.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
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

/**
Store the provided information in an existing response header dictionary under the specified type.
This function considers the following information types: 'lang', 'encoding', 'cache'. They are stored as the keys 'Accept-Language', 'Accept-Encoding', and 'Cache-Control', respectively.
Information content is read from user requests.
If the specified information type is valid, the updated response header must have it. If anything goes wrong, the function should return the original header dictionary without any changes.

@param info_type The type of information to store in the header.
@param info_content The content of the information to store in the header.
@param header The existing response header dictionary.

@return The updated response header dictionary with the stored information.
*/
HeaderDict* store_info_in_header(const char *info_type, const char *info_content, HeaderDict *header) {
    // Check if the header has enough space
    if (header->count >= HEADER_SIZE) {
        return header;
    }

    // Determine the key based on the info_type
    const char *key = NULL;
    if (strcmp(info_type, "lang") == 0) {
        key = "Accept-Language";
    } else if (strcmp(info_type, "encoding") == 0) {
        key = "Accept-Encoding";
    } else if (strcmp(info_type, "cache") == 0) {
        key = "Cache-Control";
    } else {
        return header;
    }

    // Find the index of the key if it already exists
    int index = find_header_index(header, key);

    // If the key does not exist, add it
    if (index == -1) {
        header->entries[header->count].key[0] = '\0';
        strncpy(header->entries[header->count].key, key, sizeof(header->entries[header->count].key));
        header->entries[header->count].value[0] = '\0';
        strncpy(header->entries[header->count].value, info_content, sizeof(header->entries[header->count].value));
        header->count++;
    } else {
        // If the key exists, update its value
        strncpy(header->entries[index].value, info_content, sizeof(header->entries[index].value));
    }

    return header;
}

// BEGIN ENTRYPOINT
// Function to parse the header from a string argument (like a Python dict representation)
bool parse_header(const char *input, HeaderDict *header) {
    char temp_input[1024];
    strncpy(temp_input, input, sizeof(temp_input) - 1);
    temp_input[sizeof(temp_input) - 1] = '\0';

    char *line = strtok(temp_input, ",{ }:");
    int i = 0;
    while (line != NULL && i < HEADER_SIZE) {
        if (i % 2 == 0) {
            // First element, key
            strncpy(header->entries[i / 2].key, line + 1, sizeof(header->entries[i / 2].key) - 1); // Skip the first quote
            header->entries[i / 2].key[strlen(header->entries[i / 2].key) - 1] = '\0';  // Remove last quote
        } else {
            // Second element, value
            strncpy(header->entries[i / 2].value, line + 1, sizeof(header->entries[i / 2].value) - 1); // Skip the first quote
            header->entries[i / 2].value[strlen(header->entries[i / 2].value) - 1] = '\0';  // Remove last quote
        }
        line = strtok(NULL, ",{ }:");
        i++;
    }
    header->count = i / 2;
    return true;
}

// Function to print header dictionary in Python dict format
void print_header_in_python_format(const HeaderDict *header) {
    printf("{");
    for (int i = 0; i < header->count; i++) {
        printf("\"%s\": \"%s\"", header->entries[i].key, header->entries[i].value);
        if (i < header->count - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <info_type> <info_content> '<header_dict>'\n", argv[0]);
        return 1;
    }

    const char *info_type = argv[1];
    const char *info_content = argv[2];

    // Parse the input header from the command line argument
    HeaderDict header = { .count = 0 };
    if (!parse_header(argv[3], &header)) {
        printf("Error parsing header input.\n");
        return 1;
    }

    store_info_in_header(info_type, info_content, &header);

    // Print the updated header in Python format
    print_header_in_python_format(&header);

    return 0;
}
