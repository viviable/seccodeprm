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
    const unsigned int iterations = 10000;

    for (int i = 0; i < count; ++i) {
        std::vector<unsigned char> salt(salt_len);
        if (!RAND_bytes(salt.data(), salt_len)) {
            // Handle error: could not generate random bytes
            continue;
        }

        std::vector<unsigned char> hashed_password(hash_len);
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt.data(), salt_len, iterations, EVP_sha256(), hash_len, hashed_password.data()) != 1) {
            // Handle error: PBKDF2_HMAC failed
            continue;
        }

        hashed_passwords.push_back({hashed_password, salt});
    }

    return hashed_passwords;
}
```