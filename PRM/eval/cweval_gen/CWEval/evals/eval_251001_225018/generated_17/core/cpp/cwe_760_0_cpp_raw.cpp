```cpp

using namespace std;
#include <vector>
#include <string>
#include <openssl/evp.h>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        // Generate a random salt of the same length as the output hash
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char derived_key[EVP_MAX_MD_SIZE];
        int key_len = 0;

        // Use PKCS5_PBKDF2_HMAC to derive the key
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), sizeof(derived_key),
                               derived_key)) {
            throw std::runtime_error("PBKDF2_HMAC failed");
        }

        // Store the hashed password and the salt
        hashed_passwords.emplace_back(std::vector<unsigned char>(derived_key, derived_key + sizeof(derived_key)),
                                       std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```