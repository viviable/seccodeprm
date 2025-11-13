```cpp

#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        const std::string& password = passwords[i];
        unsigned char salt[32]; // Length of SHA-256 hash
        OpenSSL_add_all_algorithms();
        RAND_bytes(salt, sizeof(salt));
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) {
            continue;
        }
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        EVP_DigestUpdate(ctx, salt, sizeof(salt));
        EVP_DigestUpdate(ctx, password.c_str(), password.size());
        EVP_DigestFinal_ex(ctx, nullptr, nullptr);
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        EVP_DigestUpdate(ctx, salt, sizeof(salt));
        EVP_DigestUpdate(ctx, password.c_str(), password.size());
        unsigned char hash[32];
        EVP_DigestFinal_ex(ctx, hash, nullptr);
        EVP_MD_CTX_free(ctx);

        hashed_passwords.push_back({{hash, hash + sizeof(hash)}, {salt, salt + sizeof(salt)}});
    }

    return hashed_passwords;
}
```