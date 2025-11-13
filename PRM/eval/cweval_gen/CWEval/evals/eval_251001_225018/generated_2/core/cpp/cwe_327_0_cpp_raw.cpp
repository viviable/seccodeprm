```cpp

using namespace std;
#include <iostream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/md5.h>

std::string bytesToHexStr(const unsigned char* bytes, size_t length) {
    std::ostringstream oss;
    for (size_t i = 0; i < length; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
    }
    return oss.str();
}

std::string hashCertificate(const std::string& cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    if (!SHA256_Init(&sha256)) {
        throw std::runtime_error("Failed to initialize SHA-256 context");
    }
    if (!SHA256_Update(&sha256, cert.c_str(), cert.size())) {
        throw std::runtime_error("Failed to update SHA-256 context");
    }
    if (!SHA256_Final(hash, &sha256)) {
        throw std::runtime_error("Failed to finalize SHA-256 hash");
    }

    return bytesToHexStr(hash, sizeof(hash));
}
```