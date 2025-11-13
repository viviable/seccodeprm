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

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    const int salt_length = EVP_sha256_size();
    unsigned char salt[salt_length];

    for (int i = 0; i < count; ++i) {
        // Generate a random salt
        if (!RAND_bytes(salt, salt_length)) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        // Hash the password using PBKDF2-HMAC-SHA256
        std::vector<unsigned char> hashed_password(EVP_MAX_MD_SIZE);
        int hashed_password_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt, salt_length, 10000, EVP_sha256(), hashed_password.data(), &hashed_password_len)) {
            throw std::runtime_error("PBKDF2_HMAC failed.");
        }
        hashed_password.resize(hashed_password_len);

        // Store the hashed password and the salt
        hashed_passwords.emplace_back(hashed_password, std::vector<unsigned char>(salt, salt + salt_length));
    }

    return hashed_passwords;
}
```