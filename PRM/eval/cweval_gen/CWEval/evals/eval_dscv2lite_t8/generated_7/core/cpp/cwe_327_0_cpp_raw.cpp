```cpp

using namespace std;
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>

std::string bytesToHexStr(const unsigned char* bytes, size_t length) {
    std::stringstream ss;
    for(size_t i = 0; i < length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]);
    }
    return ss.str();
}

std::string hashCertificate(const std::string& cert) {
    unsigned char md5Digest[MD5_DIGEST_LENGTH];
    MD5_CTX md5Ctx;
    MD5_Init(&md5Ctx);
    MD5_Update(&md5Ctx, cert.c_str(), cert.size());
    MD5_Final(md5Digest, &md5Ctx);

    return bytesToHexStr(md5Digest, MD5_DIGEST_LENGTH);
}
```