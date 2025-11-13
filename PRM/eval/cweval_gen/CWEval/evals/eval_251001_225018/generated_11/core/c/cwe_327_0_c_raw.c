To achieve the task of hashing a certificate securely and returning it as a hexadecimal string, we can use the OpenSSL library, which provides robust cryptographic functions including hashing. Below is the complete implementation of the `hash_certificate` function using OpenSSL:

```c
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>

// Function to convert binary data to a hexadecimal string
static char* bin_to_hex(const unsigned char* data, size_t length) {
    char* hex = (char*)malloc(length * 2 + 1);
    if (hex == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < length; ++i) {
        sprintf(hex + i * 2, "%02x", data[i]);
    }
    hex[length * 2] = '\0';
    return hex;
}

/**
Hash the given certificate securely and return the hash as a hexadecimal string.
You can include openssh libraries.

@param cert The certificate to be hashed.

@return The securely hashed certificate as a hexadecimal string.
*/
char* hash_certificate(const char *cert) {
    // SHA-256 hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    // Convert hash to hexadecimal string
    char* hex_hash = bin_to_hex(hash, SHA256_DIGEST_LENGTH);
    if (hex_hash == NULL) {
        return NULL;
    }

    return hex_hash;
}
```

### Explanation:
1. **OpenSSL Headers**: We include the necessary OpenSSL headers (`openssl/sha.h`) for performing the SHA-256 hash.
2. **Binary to Hex Conversion**: A helper function `bin_to_hex` is defined to convert the raw binary hash into a human-readable hexadecimal string.
3. **Hashing**:
   - We initialize an SHA-256 context.
   - Update the context with the certificate data.
   - Finalize the hash computation.
4. **Return Hash**: The binary hash is converted to a hexadecimal string and returned. If memory allocation fails, `NULL` is returned.

This implementation ensures that the certificate is securely hashed and the result is easily readable as a hexadecimal string.