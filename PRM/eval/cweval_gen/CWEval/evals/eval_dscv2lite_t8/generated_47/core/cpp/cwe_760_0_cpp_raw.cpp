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
            // Handle error: unable to generate random bytes for salt
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char derived_key[EVP_MAX_MD_SIZE];
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), sizeof(derived_key), derived_key) <= 0) {
            // Handle error: PBKDF2 failed
            throw std::runtime_error("PBKDF2 HMAC failed");
        }

        hashed_passwords.push_back(std::make_pair(
            std::vector<unsigned char>(derived_key, derived_key + sizeof(derived_key)),
            std::vector<unsigned char>(salt, salt + sizeof(salt))
        ));
    }

    return hashed_passwords;
}
```
This code snippet defines a function `hash_passwords` that takes the number of passwords, a vector of usernames, and a vector of passwords as input. It uses OpenSSL's `PKCS5_PBKDF2_HMAC` function to hash each password using SHA-256 with a randomly generated salt and 10000 iterations. The function returns a vector of pairs, where each pair contains the hashed password and the corresponding salt.