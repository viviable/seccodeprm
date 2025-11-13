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
            // Handle error: could not generate random bytes
            continue;
        }

        unsigned char key[EVP_MAX_MD_SIZE];
        int key_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), EVP_MAX_MD_SIZE, key, &key_len)) {
            // Handle error: PBKDF2_HMAC failed
            continue;
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(key, key + key_len),
                                     std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```