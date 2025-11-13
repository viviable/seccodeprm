```cpp

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;

    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The number of passwords must match the number of usernames.");
    }

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char *hashed_password;
        int hashed_length;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt), 10000,
                               EVP_sha256(), EVP_MAX_MD_SIZE, &hashed_password)) {
            throw std::runtime_error("PBKDF2 HMAC failed.");
        }

        std::vector<unsigned char> hashed_password_vec(hashed_password, hashed_password + EVP_MAX_MD_SIZE);
        std::vector<unsigned char> salt_vec(salt, salt + sizeof(salt));

        free(hashed_password); // Free the memory allocated by PKCS5_PBKDF2_HMAC

        hashed_passwords.emplace_back(std::move(hashed_password_vec), std::move(salt_vec));
    }

    return hashed_passwords;
}
```