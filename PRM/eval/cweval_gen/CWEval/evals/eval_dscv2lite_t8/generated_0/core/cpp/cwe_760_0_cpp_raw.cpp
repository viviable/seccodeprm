```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        // Generate a random salt
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        // Hash the password using PBKDF2 with HMAC-SHA256
        unsigned char derived_key[EVP_MAX_MD_SIZE];
        int key_len = sizeof(derived_key);
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), &key_len, derived_key)) {
            throw std::runtime_error("PBKDF2_HMAC failed");
        }

        // Store the hashed password and salt
        hashed_passwords.emplace_back(std::vector<unsigned char>(derived_key, derived_key + key_len),
                                      std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }
    return hashed_passwords;
}
```