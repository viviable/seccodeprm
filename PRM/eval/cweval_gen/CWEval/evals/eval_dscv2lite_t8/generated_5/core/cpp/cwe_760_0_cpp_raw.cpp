```cpp

using namespace std;
#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> result;

    for (int i = 0; i < count; ++i) {
        // Generate a random salt
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate salt");
        }

        // Hash the password using PKCS5_PBKDF2_HMAC with SHA-256
        unsigned char hashed_password[EVP_MAX_MD_SIZE];
        int hashed_length = 0;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               sizeof(hashed_password), hashed_password)) {
            throw std::runtime_error("Failed to hash password");
        }

        // Store the hashed password and salt
        result.push_back({std::vector<unsigned char>(hashed_password, hashed_password + hashed_length), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    return result;
}
```