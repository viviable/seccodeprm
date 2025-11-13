To generate a private RSA key and export it using the PEM format in C, you can use the OpenSSL library. Below is a complete implementation of the `create_private_key` function that generates an RSA private key with a specified bit length and returns it as a string in PEM format.

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

void handle_errors() {
    ERR_print_errors_fp(stderr);
    abort();
}

char *create_private_key(int key_length) {
    RSA *rsa = RSA_new();
    BIGNUM *bne = BN_new();

    if (!BN_set_word(bne, RSA_F4)) { // Use F4 (public exponent 65537)
        handle_errors();
    }

    if (!RSA_generate_key_ex(rsa, key_length, bne, NULL)) {
        handle_errors();
    }

    BIO *bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL)) {
        handle_errors();
    }

    char *buffer;
    long len = BIO_get_mem_data(bio, &buffer);
    char *pem_key = malloc(len + 1);
    if (!pem_key) {
        handle_errors();
    }
    memcpy(pem_key, buffer, len);
    pem_key[len] = '\0';

    BIO_free_all(bio);
    RSA_free(rsa);
    BN_free(bne);

    return pem_key;
}
```

### Explanation:
1. **Dependencies**: The code includes necessary headers from the OpenSSL library.
2. **Error Handling**: A helper function `handle_errors` is defined to print errors and terminate the program.
3. **RSA Key Generation**:
   - An RSA object is created using `RSA_new`.
   - A BIGNUM object is created to set the public exponent (F4).
   - The RSA key is generated using `RSA_generate_key_ex`.
4. **PEM Export**:
   - A BIO object is created to hold the memory buffer.
   - The RSA private key is written to the BIO in PEM format using `PEM_write_bio_RSAPrivateKey`.
   - The data from the BIO is copied to a dynamically allocated buffer.
5. **Cleanup**:
   - All allocated resources are freed to prevent memory leaks.

This function will generate an RSA private key with the specified bit length and return it as a string in PEM format. If any error occurs during the process, it will call `handle_errors` to print the error and terminate the program.