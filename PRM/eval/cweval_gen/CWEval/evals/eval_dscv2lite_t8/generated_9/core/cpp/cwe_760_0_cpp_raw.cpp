```cpp
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        // Generate a random salt of the same length as the output hash (32 bytes for SHA-256)
        unsigned char salt[32];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        // Hash the password using PBKDF2-HMAC-SHA256
        unsigned char hashed_password[32];
        int derived_key_len = 32; // Length of the derived key
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                              salt, sizeof(salt), 10000,
                              EVP_sha256(), derived_key_len, hashed_password) <= 0) {
            throw std::runtime_error("PBKDF2-HMAC-SHA256 failed");
        }

        // Store the hashed password and the salt
        hashed_passwords.emplace_back(std::vector<unsigned char>(hashed_password, hashed_password + derived_key_len),
                                       std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }
    return hashed_passwords;
}
```