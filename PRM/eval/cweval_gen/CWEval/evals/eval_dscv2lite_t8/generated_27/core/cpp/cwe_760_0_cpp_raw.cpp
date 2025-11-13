```cpp

#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> result;
    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The size of usernames and passwords must match the count.");
    }

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        unsigned char derived_key[EVP_MAX_MD_SIZE];
        int key_length;

        // Generate a random salt
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        // Hash the password using PBKDF2-HMAC-SHA256
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), sizeof(derived_key),
                               derived_key, &key_length)) {
            throw std::runtime_error("PBKDF2-HMAC-SHA256 failed");
        }

        // Store the result
        result.push_back({std::vector<unsigned char>(derived_key, derived_key + key_length), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    return result;
}
```