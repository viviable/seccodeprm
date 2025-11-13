```cpp
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            // Handle error: Unable to generate random bytes
            continue;
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), sizeof(hash), hash)) {
            // Handle error: PKCS5_PBKDF2_HMAC failed
            continue;
        }

        hashed_passwords.push_back({{hash, static_cast<size_t>(len)}, {salt, sizeof(salt)}});
    }

    return hashed_passwords;
}
```