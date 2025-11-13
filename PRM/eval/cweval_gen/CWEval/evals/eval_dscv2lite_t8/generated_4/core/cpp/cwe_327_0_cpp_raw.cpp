```cpp

using namespace std;
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/md5.h>

// Function to convert byte array to hexadecimal string
std::string bytesToHex(const unsigned char* bytes, size_t length) {
    std::stringstream hexStream;
    hexStream << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        hexStream << std::setw(2) << static_cast<int>(bytes[i]);
    }
    return hexStream.str();
}

// Function to hash the given certificate securely and return the hash as a hexadecimal string
std::string hashCertificate(const std::string& cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert.c_str(), cert.size());
    SHA256_Final(hash, &sha256);

    return bytesToHex(hash, SHA256_DIGEST_LENGTH);
}
```