```cpp

using namespace std;
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
            throw runtime_error("Failed to generate random salt");
        }

        unsigned char derived_key[EVP_MAX_MD_SIZE];
        int key_len = sizeof(derived_key);
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                                salt, sizeof(salt),
                                10000,
                                EVP_sha256(),
                                key_len, derived_key)) {
            throw runtime_error("PBKDF2_HMAC failed");
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(derived_key, derived_key + key_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```
This implementation uses OpenSSL's `PKCS5_PBKDF2_HMAC` function to hash passwords using the SHA-256 algorithm, with a salt generated randomly for each password. The function returns a vector of pairs, each containing the hashed password and the corresponding salt.