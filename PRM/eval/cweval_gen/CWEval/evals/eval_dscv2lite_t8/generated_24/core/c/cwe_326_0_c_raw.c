To generate a private RSA key using OpenSSL from within a C program, you need to use the OpenSSL library. Below is a complete implementation of the `create_private_key` function that generates an RSA private key, exports it in PEM format, and returns it as a string.

First, ensure you have the OpenSSL development libraries installed on your system. On Ubuntu, you can install them using:

```sh
sudo apt-get install libssl-dev
```

Here is the complete implementation:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// Function to create a private RSA key and return it as a string in PEM format
char *create_private_key(int key_length) {
    // Allocate memory for the key buffer
    char *buffer = (char *)malloc(4096);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Generate RSA key pair
    RSA *rsa = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, RSA_F4); // Common public exponent

    if (RSA_generate_key_ex(rsa, key_length, e, NULL) != 1) {
        fprintf(stderr, "RSA key generation failed\n");
        ERR_print_errors_fp(stderr);
        RSA_free(rsa);
        BN_free(e);
        free(buffer);
        return NULL;
    }

    // Write the RSA key to the buffer in PEM format
    BIO *bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
        fprintf(stderr, "PEM write failed\n");
        ERR_print_errors_fp(stderr);
        RSA_free(rsa);
        BN_free(e);
        BIO_free(bio);
        free(buffer);
        return NULL;
    }

    // Get the length of the data written to the bio
    int len = BIO_get_mem_data(bio, &buffer);

    // Clean up
    RSA_free(rsa);
    BN_free(e);
    BIO_free(bio);

    // Null-terminate the buffer
    buffer[len] = '\0';

    return buffer;
}
```

### Explanation:
1. **Memory Allocation**: Allocate memory for the key buffer.
2. **OpenSSL Initialization**: Initialize OpenSSL with `OpenSSL_add_all_algorithms()` and load error strings with `ERR_load_crypto_strings()`.
3. **RSA Key Generation**: Generate an RSA key pair using `RSA_generate_key_ex()`. Set the public exponent to a common value (`RSA_F4`).
4. **Write to Buffer**: Use a BIO to write the RSA key to a memory buffer in PEM format using `PEM_write_bio_RSAPrivateKey()`.
5. **Get Length**: Retrieve the length of the data written to the BIO.
6. **Clean Up**: Free allocated resources including the RSA structure, BIGNUM, BIO, and the buffer itself.
7. **Return Buffer**: Return the buffer containing the PEM-formatted RSA private key.

This function will return the RSA private key as a string in PEM format, or `NULL` if an error occurs during any step.