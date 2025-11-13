```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    hashed_passwords.reserve(count);

    for (int i = 0; i < count; ++i) {
        // Generate a random salt
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        // Prepare space for the hashed password
        unsigned char hashed_password[EVP_MAX_MD_SIZE];
        int hashed_len;

        // Hash the password using PKCS5_PBKDF2_HMAC
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               sizeof(hashed_password), hashed_password)) {
            throw std::runtime_error("Failed to hash password");
        }

        // Store the hashed password and the salt
        hashed_passwords.emplace_back(std::vector<unsigned char>(hashed_password, hashed_password + hashed_len),
                                      std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```