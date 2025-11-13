```cpp

using namespace std;
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>

std::string hashCertificate(const std::string& cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert.c_str(), cert.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}
```
This code uses OpenSSL's SHA-256 hashing algorithm to secure hash a given certificate. It initializes an SHA256 context, updates it with the certificate data, and then finalizes the hash. The resulting hash is converted into a hexadecimal string using stringstream and formatted with two digits per byte.