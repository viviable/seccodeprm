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
        // Generate a random salt
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt");
        }

        unsigned char key[EVP_MAX_MD_SIZE];
        int key_len;
        // Hash the password using PBKDF2 with HMAC-SHA256
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               sizeof(key), key)) {
            throw std::runtime_error("PBKDF2 failed");
        }

        // Store the hashed password and salt
        hashed_passwords.push_back({{key, key + sizeof(key)}, {salt, salt + sizeof(salt)}});
    }

    return hashed_passwords;
}
```
This code snippet includes the necessary OpenSSL headers and implements the `hash_passwords` function as described. It generates random salts, hashes passwords using PBKDF2 with HMAC-SHA256, and returns the hashed passwords along with their corresponding salts.