```cpp

using namespace std;

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The size of usernames, passwords, and count must match.");
    }

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char derived_key[EVP_MAX_MD_SIZE];
        int derived_key_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               derived_key, sizeof(derived_key),
                               &derived_key_len)) {
            throw std::runtime_error("PBKDF2 HMAC failed.");
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(derived_key, derived_key + derived_key_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```