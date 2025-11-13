```cpp

using namespace std;
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>

std::string bytesToHex(const unsigned char* bytes, size_t length) {
    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        hexStream << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return hexStream.str();
}

std::string hashCertificate(const std::string& cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    if (SHA256_Init(&sha256) == 1) {
        SHA256_Update(&sha256, cert.c_str(), cert.size());
        SHA256_Final(hash, &sha256);
    } else {
        throw std::runtime_error("Failed to initialize SHA-256 context");
    }
    return bytesToHex(hash, SHA256_DIGEST_LENGTH);
}
```