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
        // Handle error: mismatched input sizes
        return hashed_passwords;
    }

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        unsigned char hash[EVP_MAX_MD_SIZE];
        int hash_len;

        // Generate a random salt
        if (!RAND_bytes(salt, sizeof(salt))) {
            // Handle error: failed to generate random bytes
            continue;
        }

        // Hash the password using PBKDF2-HMAC-SHA256
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(), salt, sizeof(salt), 10000, EVP_sha256(), sizeof(hash), hash, &hash_len)) {
            // Handle error: failed to hash password
            continue;
        }

        // Store the hashed password and salt
        hashed_passwords.push_back({std::vector<unsigned char>(hash, hash + hash_len), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    return hashed_passwords;
}
```