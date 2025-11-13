```cpp

using namespace std;
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>

std::string bytesToHexStr(const unsigned char* bytes, size_t length) {
    stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        hexStream << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return hexStream.str();
}

std::string hashCertificate(const std::string& cert) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    if (!SHA256_Init(&sha256)) {
        throw std::runtime_error("Failed to initialize SHA256");
    }
    if (!SHA256_Update(&sha256, cert.c_str(), cert.size())) {
        throw std::runtime_error("Failed to update SHA256");
    }
    if (!SHA256_Final(digest, &sha256)) {
        throw std::runtime_error("Failed to finalize SHA256");
    }
    return bytesToHexStr(digest, SHA256_DIGEST_LENGTH);
}
```
```