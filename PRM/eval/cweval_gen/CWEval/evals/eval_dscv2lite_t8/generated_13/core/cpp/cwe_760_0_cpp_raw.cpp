```cpp

using namespace std;

#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create MD context");
    }

    for (int i = 0; i < count; ++i) {
        const unsigned char* password = reinterpret_cast<const unsigned char*>(passwords[i].c_str());
        size_t password_len = passwords[i].length();

        // Generate a random salt of the same length as the output hash
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("Failed to generate salt");
        }

        // Hash the password using PBKDF2-HMAC-SHA256
        unsigned char hash[EVP_MAX_MD_SIZE];
        if (!PKCS5_PBKDF2_HMAC(password, static_cast<int>(password_len), salt, sizeof(salt), 10000, EVP_sha256(), hash)) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("PBKDF2-HMAC-SHA256 failed");
        }

        hashed_passwords.push_back({std::vector<unsigned char>(hash, hash + EVP_sha256()->md_size), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }

    EVP_MD_CTX_free(ctx);
    return hashed_passwords;
}
```