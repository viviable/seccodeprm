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
        std::vector<unsigned char> salt(salt_len);
        if (!RAND_bytes(salt.data(), salt.size())) {
            // Handle error: failed to generate random bytes
            continue;
        }

        std::vector<unsigned char> hashed_password(hash_len);
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt.data(), salt.size(), 10000, EVP_sha256(), hash_len, hashed_password.data()) <= 0) {
            // Handle error: PKCS5_PBKDF2_HMAC failed
            continue;
        }

        hashed_passwords.emplace_back(hashed_password, salt);
    }

    return hashed_passwords;
}
```