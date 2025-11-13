```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        const std::string& password = passwords[i];
        unsigned char salt[32]; // SHA-256 produces a 256-bit hash, so we need a 32-byte salt
        RAND_bytes(salt, sizeof(salt));

        unsigned char hashed_password[EVP_MAX_MD_SIZE];
        int hashed_length;
        PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt, sizeof(salt), 10000, EVP_sha256(), EVP_MAX_MD_SIZE, hashed_password);

        std::vector<unsigned char> hashed_password_vec(hashed_password, hashed_password + hashed_length);
        std::vector<unsigned char> salt_vec(salt, salt + sizeof(salt));

        hashed_passwords.push_back(std::make_pair(hashed_password_vec, salt_vec));
    }

    return hashed_passwords;
}
```