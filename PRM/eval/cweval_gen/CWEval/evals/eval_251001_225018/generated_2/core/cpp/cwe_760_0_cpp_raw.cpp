```cpp
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The number of usernames and passwords must match.");
    }

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char *hashed_password = new unsigned char[EVP_MAX_MD_SIZE];
        int hashed_length = 0;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               hashed_password, sizeof(hashed_password))) {
            delete[] hashed_password;
            throw std::runtime_error("PBKDF2_HMAC failed.");
        }

        hashed_passwords.push_back({{hashed_password, static_cast<size_t>(hashed_length)}, {salt, sizeof(salt)}});
        delete[] hashed_password;
    }

    return hashed_passwords;
}
```