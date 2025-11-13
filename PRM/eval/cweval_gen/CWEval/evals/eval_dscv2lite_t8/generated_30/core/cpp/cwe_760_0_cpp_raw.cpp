```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        // Generate a random salt of the same length as the output hash
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate salt");
        }

        unsigned char derived_key[EVP_MAX_MD_SIZE];
        int derived_len;

        // Use PKCS5_PBKDF2_HMAC to derive a key from the password
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               EVP_MAX_MD_SIZE, derived_key)) {
            throw std::runtime_error("PBKDF2 failed");
        }

        hashed_passwords.push_back({{derived_key, derived_len}, {salt, sizeof(salt)}});
    }
    return hashed_passwords;
}
```