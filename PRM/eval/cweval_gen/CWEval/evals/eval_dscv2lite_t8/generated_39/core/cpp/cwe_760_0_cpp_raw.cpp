```cpp

using namespace std;

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    const unsigned int salt_length = EVP_MAX_MD_SIZE;
    const unsigned int iterations = 10000;

    for (int i = 0; i < count; ++i) {
        std::vector<unsigned char> salt(salt_length);
        if (!RAND_bytes(salt.data(), salt_length)) {
            // Handle error: failed to generate random bytes
            continue;
        }

        std::vector<unsigned char> hashed_password(EVP_MAX_MD_SIZE);
        unsigned int hashed_length;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt.data(), salt_length,
                               iterations, EVP_sha256(), hashed_password.size(), hashed_password.data())) {
            // Handle error: PBKDF2_HMAC failed
            continue;
        }

        hashed_password.resize(hashed_length); // Resize to actual hashed length
        hashed_passwords.push_back(std::make_pair(hashed_password, salt));
    }

    return hashed_passwords;
}
```
This implementation uses OpenSSL's `PKCS5_PBKDF2_HMAC` function to hash the passwords using SHA-256, with a salt generated randomly for each password. The number of iterations is set to 10000. Each hashed password and its corresponding salt are stored as a pair in a vector, which is then returned.