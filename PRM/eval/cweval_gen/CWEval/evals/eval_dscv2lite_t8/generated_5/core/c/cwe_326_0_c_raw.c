To generate a private RSA key using OpenSSL in C, you will need to use the OpenSSL library. Below is a complete implementation of the `create_private_key` function that generates a private RSA key and returns it in PEM format as a string. Note that this code assumes that OpenSSL is installed and properly configured on your system.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    RSA *rsa = NULL;
    BIO *bio = NULL;
    char *buffer = NULL;
    int buffer_len;

    // Generate RSA key pair
    rsa = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, RSA_F4); // Public exponent
    RSA_generate_key_ex(rsa, KEY_LENGTH, e, NULL);

    // Create a BIO memory buffer to hold the key data
    bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
        BIO_free_all(bio);
        RSA_free(rsa);
        BN_free(e);
        return NULL;
    }

    // Get the length of the key data
    buffer_len = BIO_get_mem_data(bio, &buffer);
    if (buffer_len <= 0) {
        BIO_free_all(bio);
        RSA_free(rsa);
        BN_free(e);
        return NULL;
    }

    // Allocate memory for the key data
    char *private_key = (char *)malloc(buffer_len + 1);
    if (!private_key) {
        BIO_free_all(bio);
        RSA_free(rsa);
        BN_free(e);
        return NULL;
    }

    // Copy the key data to the allocated memory
    memcpy(private_key, buffer, buffer_len);
    private_key[buffer_len] = '\0';

    // Clean up
    BIO_free_all(bio);
    RSA_free(rsa);
    BN_free(e);

    return private_key;
}
```

### Explanation:
1. **RSA Key Generation**:
   - `RSA_new()` creates a new RSA structure.
   - `BN_new()` creates a new BIGNUM structure for the public exponent.
   - `BN_set_word(e, RSA_F4)` sets the public exponent to `65537`.
   - `RSA_generate_key_ex(rsa, KEY_LENGTH, e, NULL)` generates an RSA key pair with the specified key length and public exponent.

2. **BIO Memory Buffer**:
   - `BIO_new(BIO_s_mem())` creates a new BIO memory buffer.
   - `PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)` writes the RSA private key to the BIO memory buffer in PEM format.

3. **Retrieve Key Data**:
   - `BIO_get_mem_data(bio, &buffer)` retrieves the key data from the BIO memory buffer.
   - If successful, it allocates memory for the key data and copies the data into it.

4. **Cleanup**:
   - Frees the BIO memory buffer, RSA structure, and BIGNUM structure.

This function returns the RSA private key in PEM format as a string, or `NULL` if an error occurs during the generation process.