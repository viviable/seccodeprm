```cpp
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    if (count != usernames.size() || count != passwords.size()) {
        // Handle error: mismatched input sizes
        return hashed_passwords;
    }

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            // Handle error: failed to generate random salt
            continue;
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(), salt, sizeof(salt), 10000, EVP_sha256(), sizeof(hash), hash)) {
            // Handle error: PBKDF2_HMAC failed
            continue;
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```
This code snippet includes the necessary OpenSSL headers and defines the `hash_passwords` function as specified. It uses PKCS5_PBKDF2_HMAC to hash each password with SHA-256, generates a unique salt for each password, and performs 10000 iterations. The function returns a vector of pairs, each containing the hashed password and the corresponding salt. Error handling is included for cases where inputs do not match expected sizes or when generating random bytes or performing the hashing process fails.