To achieve this task, we need to use OpenSSL library which provides cryptographic functions including hashing. Here's how you can implement the `hash_certificate` function:

```c
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>

// Function to convert bytes to hexadecimal string
static void bytes_to_hex_string(const unsigned char *bytes, size_t length, char *hex_string) {
    for (size_t i = 0; i < length; ++i) {
        sprintf(hex_string + i * 2, "%02x", bytes[i]);
    }
}

char* hash_certificate(const char *cert) {
    // Initialize SHA-256 context
    SHA256_CTX sha256;
    if (!SHA256_Init(&sha256)) {
        return NULL; // Handle error
    }

    // Update the context with the certificate data
    if (!SHA256_Update(&sha256, cert, strlen(cert))) {
        return NULL; // Handle error
    }

    // Finalize the hash computation
    unsigned char digest[SHA256_DIGEST_LENGTH];
    if (!SHA256_Final(digest, &sha256)) {
        return NULL; // Handle error
    }

    // Allocate memory for the hexadecimal string
    char *hex_string = malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (!hex_string) {
        return NULL; // Handle memory allocation failure
    }

    // Convert the hash to a hexadecimal string
    bytes_to_hex_string(digest, SHA256_DIGEST_LENGTH, hex_string);

    return hex_string;
}
```

This code includes the necessary OpenSSL headers and defines a helper function `bytes_to_hex_string` to convert the raw bytes of the hash into a hexadecimal string. The `hash_certificate` function initializes an SHA-256 context, updates it with the certificate data, finalizes the hash computation, and then converts the resulting digest to a hexadecimal string. The allocated memory for the hexadecimal string is returned. Error handling is included for each step that could fail.