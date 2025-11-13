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
        // Generate a random salt
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate salt");
        }

        unsigned char hashed_password[EVP_MAX_MD_SIZE];
        int hashed_length;
        if (!PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt, sizeof(salt), 10000, EVP_sha256(), sizeof(hashed_password), hashed_password)) {
            throw std::runtime_error("PBKDF2 HMAC failed");
        }

        hashed_passwords.push_back({std::vector<unsigned char>(hashed_password, hashed_password + sizeof(hashed_password)), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    return hashed_passwords;
}
```