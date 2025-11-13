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
        // Generate a random salt of the same length as the output hash
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate salt");
        }

        unsigned char derived_key[EVP_MAX_MD_SIZE];
        // Use PKCS5_PBKDF2_HMAC to derive a key from the password
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                              salt, sizeof(salt),
                              10000,
                              EVP_sha256(), sizeof(derived_key), derived_key) <= 0) {
            throw std::runtime_error("PKCS5_PBKDF2_HMAC failed");
        }

        hashed_passwords.push_back({{derived_key, derived_key + sizeof(derived_key)}, {salt, salt + sizeof(salt)}});
    }
    return hashed_passwords;
}
```
This code snippet defines a function `hash_passwords` that takes the number of passwords, usernames, and passwords themselves as input. It uses OpenSSL's `PKCS5_PBKDF2_HMAC` function to hash each password using SHA-256, a randomly generated salt of the same length as the hash output, and 10000 iterations. The function returns a vector of pairs, where each pair contains the hashed password and the corresponding salt.