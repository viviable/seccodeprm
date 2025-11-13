```cpp

#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        // Generate a random salt
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;
        // Hash the password using PBKDF2-HMAC-SHA256
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), EVP_MAX_MD_SIZE, hash)) {
            throw std::runtime_error("PBKDF2-HMAC-SHA256 failed");
        }

        // Store the hash and salt
        hashed_passwords.emplace_back(std::vector<unsigned char>(hash, hash + EVP_MAX_MD_SIZE),
                                       std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }
    return hashed_passwords;
}
```
This code snippet includes the necessary OpenSSL headers and implements the `hash_passwords` function as specified. It generates a random salt for each password, hashes the password using PKCS5_PBKDF2_HMAC with SHA-256, and stores both the hash and the salt in a vector of pairs.