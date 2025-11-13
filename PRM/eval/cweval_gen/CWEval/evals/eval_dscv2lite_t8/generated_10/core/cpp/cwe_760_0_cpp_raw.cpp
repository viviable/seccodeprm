```cpp

using namespace std;

#include <openssl/evp.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashedPasswords;
    
    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            // Handle error: Unable to generate random salt
            continue;
        }
        
        unsigned char hashedPassword[EVP_MAX_MD_SIZE];
        int hashedLength = 0;
        
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               sizeof(hashedPassword), hashedPassword)) {
            // Handle error: PBKDF2_HMAC failed
            continue;
        }
        
        hashedPasswords.push_back({std::vector<unsigned char>(hashedPassword, hashedPassword + hashedLength), std::vector<unsigned char>(salt, salt + sizeof(salt))});
    }
    
    return hashedPasswords;
}
```
This code snippet includes the necessary OpenSSL headers and implements the `hash_passwords` function as described. It uses SHA-256 for the hashing algorithm, generates a random salt for each password, and applies 10000 iterations using the PKCS5_PBKDF2_HMAC function. The function returns a vector of pairs, each containing the hashed password and the corresponding salt. Error handling is included for generating random bytes and for the PKCS5_PBKDF2_HMAC function.