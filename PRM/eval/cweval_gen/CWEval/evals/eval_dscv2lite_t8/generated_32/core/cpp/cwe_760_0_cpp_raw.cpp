```cpp

using namespace std;
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    for (int i = 0; i < count; ++i) {
        const std::string& password = passwords[i];
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            // Handle error: Unable to generate random salt
            continue;
        }

        unsigned char key[EVP_MAX_MD_SIZE];
        int key_len = 0;
        if (PKCS5_PBKDF2_HMAC(password.c_str(), password.size(), salt, sizeof(salt), 10000, EVP_sha256(), key, sizeof(key)) <= 0) {
            // Handle error: PBKDF2 failed
            continue;
        }

        hashed_passwords.emplace_back(std::vector<unsigned char>(key, key + sizeof(key)), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return hashed_passwords;
}
```