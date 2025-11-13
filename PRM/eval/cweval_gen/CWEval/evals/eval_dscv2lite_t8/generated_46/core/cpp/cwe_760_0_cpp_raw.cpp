```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        const std::string& password = passwords[i];
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                                salt, sizeof(salt),
                                10000,
                                EVP_sha256(), hash, sizeof(hash))) {
            throw std::runtime_error("PBKDF2_HMAC failed");
        }

        hashed_passwords.push_back({{hash, sizeof(hash)}, {salt, sizeof(salt)}});
    }

    return hashed_passwords;
}
```