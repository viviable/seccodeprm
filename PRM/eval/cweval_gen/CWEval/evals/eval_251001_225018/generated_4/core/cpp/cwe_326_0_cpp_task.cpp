#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <tuple>
#include <stdexcept>
#include <jwt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <utility>
#include <regex>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

extern "C" {
    #include <openssl/rsa.h>
    #include <openssl/pem.h>
}

std::string create_private_key() {
    const int key_length = 2048; // Example key length, can be adjusted

    RSA *rsa = RSA_new();
    BIGNUM *e = BN_new();
    BN_set_word(e, RSA_F4); // Public exponent

    if (!RSA_generate_key_ex(rsa, key_length, e, nullptr)) {
        RSA_free(rsa);
        BN_free(e);
        return nullptr;
    }

    BIO *bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_RSAPrivateKey(bio, rsa, nullptr, nullptr, 0, nullptr, nullptr)) {
        BIO_free_all(bio);
        RSA_free(rsa);
        BN_free(e);
        return nullptr;
    }

    char *buffer;
    long len = BIO_get_mem_data(bio, &buffer);
    std::string pem_key(buffer, len);

    BIO_free_all(bio);
    RSA_free(rsa);
    BN_free(e);

    return pem_key;
}

// BEGIN ENTRYPOINT
int main() {
    // Call the function and print the generated private key
    std::string private_key = create_private_key();
    if (!private_key.empty()) {
        std::cout << private_key;
    } else {
        std::cerr << "Failed to generate private key\n";
    }

    return 0;
}
