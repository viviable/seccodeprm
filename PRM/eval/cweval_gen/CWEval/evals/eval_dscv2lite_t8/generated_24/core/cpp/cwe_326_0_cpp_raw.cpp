```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

std::string create_private_key(size_t key_length) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    RSA* rsa = RSA_new();
    BIGNUM* e = BN_new();
    BN_set_word(e, RSA_F4);

    if (!RSA_generate_key_ex(rsa, key_length, e, nullptr)) {
        RSA_free(rsa);
        BN_free(e);
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    BIO* bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, nullptr, nullptr, 0, nullptr, nullptr)) {
        BIO_free_all(bio);
        RSA_free(rsa);
        BN_free(e);
        ERR_print_errors_fp(stderr);
        return nullptr;
    }

    char* buffer;
    long length = BIO_get_mem_data(bio, &buffer);
    std::string pem_key(buffer, length);

    BIO_free_all(bio);
    RSA_free(rsa);
    BN_free(e);

    return pem_key;
}
```