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
            throw std::runtime_error("Failed to generate salt");
        }

        // Prepare buffer for the hashed password
        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len = 0;

        // Hash the password using PKCS5_PBKDF2_HMAC
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(), hash, sizeof(hash), &hash_len)) {
            throw std::runtime_error("PKCS5_PBKDF2_HMAC failed");
        }

        // Store the hashed password and salt
        hashed_passwords.emplace_back(std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```