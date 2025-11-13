```cpp

using namespace std;
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The size of usernames and passwords must match the count.");
    }

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char derived_key[EVP_MAX_MD_SIZE];
        int derived_key_len = 0;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(), salt, sizeof(salt), 10000, EVP_sha256(), EVP_MAX_MD_SIZE, derived_key)) {
            throw std::runtime_error("PBKDF2_HMAC failed.");
        }

        hashed_passwords.push_back({std::vector<unsigned char>(derived_key, derived_key + derived_key_len), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    return hashed_passwords;
}
```