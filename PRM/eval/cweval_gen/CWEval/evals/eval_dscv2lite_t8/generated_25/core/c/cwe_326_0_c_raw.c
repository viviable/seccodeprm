To generate a private RSA key and export it in PEM format using OpenSSL in C, you need to use the OpenSSL library functions. Below is the complete implementation of the `create_private_key` function:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// Function to create a private RSA key and return it in PEM format
char *create_private_key(int key_length) {
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Generate RSA key pair
    RSA *rsa = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, RSA_F4); // Public exponent 65537

    int result = RSA_generate_key_ex(rsa, key_length, e, NULL);
    BN_free(e);

    if (!result) {
        ERR_print_errors_fp(stderr);
        RSA_free(rsa);
        return NULL;
    }

    // Create a BIO to hold the PEM data
    BIO *bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
        ERR_print_errors_fp(stderr);
        BIO_free(bio);
        RSA_free(rsa);
        return NULL;
    }

    // Get the length of the PEM data
    long len = BIO_pending(bio);

    // Allocate memory for the PEM data
    char *pem_data = malloc(len + 1);
    if (!pem_data) {
        BIO_free(bio);
        RSA_free(rsa);
        return NULL;
    }

    // Read the PEM data from the BIO
    BIO_read(bio, pem_data, len);
    pem_data[len] = '\0';

    // Free resources
    BIO_free(bio);
    RSA_free(rsa);

    return pem_data;
}
```

This code includes the necessary OpenSSL headers and initializes the OpenSSL library before generating the RSA key. It then writes the key to a BIO, which holds the PEM formatted data. The function returns this data as a string, or `NULL` if an error occurs during the process. Remember to link against the OpenSSL library when compiling this code.