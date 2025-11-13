```cpp
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    const unsigned int hash_len = EVP_sha256_DIGEST_LENGTH;
    const unsigned int salt_len = hash_len;

    for (int i = 0; i < count; ++i) {
        unsigned char salt[salt_len];
        if (!RAND_bytes(salt, salt_len)) {
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char hash[hash_len];
        PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                          salt, salt_len, 10000,
                          EVP_sha256(), hash_len, hash);

        hashed_passwords.emplace_back(std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + salt_len));
    }

    return hashed_passwords;
}
```