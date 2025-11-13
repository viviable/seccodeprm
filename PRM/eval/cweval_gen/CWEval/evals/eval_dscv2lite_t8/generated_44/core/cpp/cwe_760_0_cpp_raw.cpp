```cpp

using namespace std;
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            // Handle error: could not generate random salt
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char key[EVP_MAX_MD_SIZE];
        int key_len;

        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), sizeof(key), key)) {
            // Handle error: PBKDF2 failed
            throw std::runtime_error("PBKDF2 HMAC failed");
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(key, key + sizeof(key)), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```