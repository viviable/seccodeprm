```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    const unsigned int hash_length = EVP_sha256() ? EVP_sha256()->md_size : 32; // Default to SHA-256 size if EVP_sha256 is not available
    const unsigned int salt_length = hash_length;

    for (int i = 0; i < count; ++i) {
        std::vector<unsigned char> salt(salt_length);
        std::vector<unsigned char> hashed_password(hash_length);

        // Generate random salt
        if (!RAND_bytes(salt.data(), salt.size())) {
            throw std::runtime_error("Failed to generate random salt");
        }

        // Hash the password using PKCS5_PBKDF2_HMAC
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt.data(), salt.size(),
                              10000, EVP_sha256(), hash_length, hashed_password.data()) != 1) {
            throw std::runtime_error("Failed to hash password");
        }

        hashed_passwords.emplace_back(hashed_password, salt);
    }

    return hashed_passwords;
}
```