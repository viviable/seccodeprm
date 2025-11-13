```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

extern "C" {
    #include <openssl/rsa.h>
    #include <openssl/pem.h>
    #include <openssl/err.h>
}

std::string create_private_key(int key_length) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    RSA *rsa = RSA_new();
    BIGNUM *bne = BN_new();
    BN_set_word(bne, RSA_F4); // Set public exponent to F4 (65537)

    int result = RSA_generate_key_ex(rsa, key_length, bne, nullptr);
    BN_free(bne);

    if (!result) {
        ERR_print_errors_fp(stderr);
        RSA_free(rsa);
        return nullptr;
    }

    std::stringstream ss;
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(bio, rsa, nullptr, nullptr, 0, nullptr, nullptr);
    char *buffer;
    long len = BIO_get_mem_data(bio, &buffer);
    ss.write(buffer, len);
    BIO_free(bio);
    RSA_free(rsa);

    return ss.str();
}
```