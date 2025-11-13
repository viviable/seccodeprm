```cpp

using namespace std;

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char hashed_password[EVP_MAX_MD_SIZE];
        int hashed_len = 0;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(), salt, sizeof(salt), 10000, EVP_sha256(), hashed_password, sizeof(hashed_password), &hashed_len)) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("PKCS5_PBKDF2_HMAC failed");
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(hashed_password, hashed_password + hashed_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    EVP_MD_CTX_free(ctx);
    return hashed_passwords;
}
```