```cpp

using namespace std;
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/md5.h>

#include <openssl/sha.h>
#include <openssl/md5.h>

static const char* hex_chars = "0123456789abcdef";

std::string bytesToHex(const unsigned char* bytes, size_t length) {
    std::stringstream ss;
    for (size_t i = 0; i < length; ++i) {
        ss << hex_chars[bytes[i] >> 4];
        ss << hex_chars[bytes[i] & 0x0F];
    }
    return ss.str();
}

std::string hashCertificate(const std::string& cert) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert.c_str(), cert.size());
    SHA256_Final(digest, &sha256);

    return bytesToHex(digest, sizeof(digest));
}
```