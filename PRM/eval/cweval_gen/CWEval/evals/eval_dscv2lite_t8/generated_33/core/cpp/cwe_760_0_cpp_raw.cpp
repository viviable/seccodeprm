```cpp

using namespace std;
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The count must match the size of usernames and passwords.");
    }

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(), hash, sizeof(hash), &hash_len)) {
            throw std::runtime_error("Failed to hash password.");
        }

        hashed_passwords.push_back({std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    return hashed_passwords;
}
```
This code snippet includes the necessary OpenSSL headers and implements the `hash_passwords` function as specified. It generates a random salt for each password, uses PKCS5_PBKDF2_HMAC with SHA-256, and returns a vector of pairs containing the hashed passwords and their corresponding salts.