```cpp

using namespace std;
#include <iostream>
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
    unsigned char md5Digest[MD5_DIGEST_LENGTH];
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    MD5_Update(&md5Context, cert.c_str(), cert.size());
    MD5_Final(md5Digest, &md5Context);

    return bytesToHex(md5Digest, MD5_DIGEST_LENGTH);
}
```