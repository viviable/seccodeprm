```cpp

using namespace std;
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> result;

    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The number of passwords must match the number of usernames.");
    }

    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt, sizeof(salt), 10000, EVP_sha256(), sizeof(hash), hash)) {
            throw std::runtime_error("PBKDF2_HMAC failed.");
        }

        result.emplace_back(std::vector<unsigned char>(hash, hash + sizeof(hash)), std::vector<unsigned char>(salt, salt + sizeof(salt)));
    }

    return result;
}
```
This implementation includes error handling for generating random salts and performing the PBKDF2 HMAC operation using OpenSSL's functions. It returns a vector of pairs, where each pair contains the hashed password and the corresponding salt.