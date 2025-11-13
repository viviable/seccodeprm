```cpp

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
            // Handle error: unable to generate random bytes
            continue;
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt, sizeof(salt), 10000, EVP_sha256(), &hash_len, hash)) {
            // Handle error: PBKDF2_HMAC failed
            continue;
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }
    return hashed_passwords;
}
```
This code snippet defines a function `hash_passwords` that takes the number of passwords, a vector of usernames, and a vector of passwords. It uses OpenSSL's `PKCS5_PBKDF2_HMAC` function to hash each password with SHA-256 using a randomly generated salt and 10000 iterations. The function returns a vector of pairs, where each pair contains the hashed password and the corresponding salt. Error handling is included for generating random bytes and the PBKDF2_HMAC operation.