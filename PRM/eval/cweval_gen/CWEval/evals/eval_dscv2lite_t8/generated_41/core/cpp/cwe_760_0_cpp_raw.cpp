```cpp

using namespace std;

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    const unsigned int hash_len = EVP_sha256() -> md_size;
    const unsigned int salt_len = hash_len;
    const unsigned int iterations = 10000;

    for (int i = 0; i < count; ++i) {
        std::vector<unsigned char> salt(salt_len);
        if (!RAND_bytes(salt.data(), salt.size())) {
            // Handle error: unable to generate random bytes
            throw std::runtime_error("Failed to generate random salt");
        }

        std::vector<unsigned char> hashed_password(hash_len);
        if (PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt.data(), salt.size(), iterations, EVP_sha256(), hash_len, hashed_password.data()) != 1) {
            // Handle error: PBKDF2 failed
            throw std::runtime_error("PBKDF2 HMAC failed");
        }

        hashed_passwords.push_back(std::make_pair(hashed_password, salt));
    }

    return hashed_passwords;
}
```