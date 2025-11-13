```cpp

using namespace std;

#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        unsigned char salt[32]; // Length of SHA-256 hash
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw runtime_error("Failed to generate random salt");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), hash_len, hash)) {
            throw runtime_error("PKCS5_PBKDF2_HMAC failed");
        }

        std::vector<unsigned char> hashed_password(hash, hash + hash_len);
        std::vector<unsigned char> hashed_salt(salt, salt + sizeof(salt));
        hashed_passwords.emplace_back(std::move(hashed_password), std::move(hashed_salt));
    }
    return hashed_passwords;
}
```