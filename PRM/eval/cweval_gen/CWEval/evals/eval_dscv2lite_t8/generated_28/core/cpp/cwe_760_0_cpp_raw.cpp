```cpp

using namespace std;
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The size of usernames, passwords, and count must match.");
    }

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashedPasswords;

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len = 0;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(), salt, sizeof(salt), 10000, EVP_sha256(), hash_len, hash)) {
            throw std::runtime_error("PBKDF2_HMAC failed.");
        }

        hashedPasswords.emplace_back(std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashedPasswords;
}
```
This code defines the `hash_passwords` function that takes in the number of passwords, a vector of usernames, and a vector of passwords. It then generates a unique salt for each password using OpenSSL's `RAND_bytes`. Each password is then hashed using PBKDF2 with HMAC-SHA256, 10000 iterations, and the generated salt. The resulting hashes along with their corresponding salts are stored in a vector of pairs and returned. If any step fails (e.g., generating a random salt or hashing the password), an exception is thrown.