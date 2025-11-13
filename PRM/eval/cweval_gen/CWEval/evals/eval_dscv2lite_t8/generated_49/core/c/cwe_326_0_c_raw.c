To generate an RSA private key using OpenSSL from within a C program, you need to use the OpenSSL library. Below is the complete implementation of the `create_private_key` function that generates an RSA private key of a specified length and exports it in PEM format:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    RSA *rsa = NULL;
    char *pem_key = NULL;

    // Generate RSA key pair
    rsa = RSA_generate_key(KEY_LENGTH, RSA_F4, NULL, NULL);
    if (!rsa) {
        fprintf(stderr, "Failed to generate RSA key\n");
        return NULL;
    }

    // Allocate memory for the PEM key
    pem_key = (char *)malloc(16384); // 16KB buffer
    if (!pem_key) {
        fprintf(stderr, "Failed to allocate memory for PEM key\n");
        RSA_free(rsa);
        return NULL;
    }

    // Write the RSA private key to the buffer in PEM format
    FILE *fp = fmemopen(pem_key, 16384, "w");
    if (!PEM_write_RSAPrivateKey(fp, rsa, NULL, NULL, 0, NULL, NULL)) {
        fprintf(stderr, "Failed to write RSA private key to PEM format\n");
        fclose(fp);
        free(pem_key);
        RSA_free(rsa);
        return NULL;
    }
    fclose(fp);

    // Null-terminate the string
    pem_key[16383] = '\0';

    // Free the RSA structure
    RSA_free(rsa);

    return pem_key;
}
```

This function uses the OpenSSL library to generate an RSA key pair and then writes the private key to a buffer in PEM format. The buffer is allocated on the heap, and the caller is responsible for freeing the returned string when it is no longer needed. If any step fails, the function returns `NULL`.