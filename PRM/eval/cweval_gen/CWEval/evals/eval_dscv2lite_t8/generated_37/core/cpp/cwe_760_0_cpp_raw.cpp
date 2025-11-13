```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        unsigned char salt[32];
        // Generate a random salt
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;

        // Use PKCS5_PBKDF2_HMAC to hash the password
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(), EVP_MAX_MD_SIZE, hash, &hash_len)) {
            throw std::runtime_error("Failed to hash password");
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }
    return hashed_passwords;
}
```