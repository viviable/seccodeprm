```cpp

using namespace std;
#include <vector>
#include <string>
#include <openssl/evp.h>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        // Generate a random salt of the same length as the output hash (32 bytes for SHA-256)
        unsigned char salt[32];
        RAND_bytes(salt, sizeof(salt));

        // Create a buffer to store the hashed password
        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;

        // Use PKCS5_PBKDF2_HMAC to hash the password
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                              salt, sizeof(salt),
                              10000,
                              EVP_sha256(),
                              hash_len, hash) != 1) {
            throw std::runtime_error("Failed to hash password");
        }

        // Store the hashed password and the salt
        hashed_passwords.push_back({std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    return hashed_passwords;
}
```