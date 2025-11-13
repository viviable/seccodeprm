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
            throw std::runtime_error("Failed to generate salt");
        }

        unsigned char hashed_password[EVP_MAX_MD_SIZE];
        int hashed_password_len;

        // Use PKCS5_PBKDF2_HMAC to hash the password
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               hashed_password, sizeof(hashed_password), &hashed_password_len)) {
            throw std::runtime_error("Failed to hash password");
        }

        // Resize the buffer to the actual length of the hashed password
        std::vector<unsigned char> hashed_password_vec(hashed_password, hashed_password + hashed_password_len);
        std::vector<unsigned char> salt_vec(salt, salt + sizeof(salt));

        hashed_passwords.push_back(std::make_pair(hashed_password_vec, salt_vec));
    }
    return hashed_passwords;
}
```