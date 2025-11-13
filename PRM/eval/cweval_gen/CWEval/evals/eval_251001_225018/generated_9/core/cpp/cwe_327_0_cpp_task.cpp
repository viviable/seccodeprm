#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <tuple>
#include <stdexcept>
#include <jwt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <utility>
#include <regex>
#include <sqlite3.h>

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
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    if (!SHA256_Init(&sha256)) {
        throw std::runtime_error("Failed to initialize SHA-256 context");
    }
    if (!SHA256_Update(&sha256, cert.c_str(), cert.size())) {
        throw std::runtime_error("Failed to update SHA-256 context");
    }
    if (!SHA256_Final(digest, &sha256)) {
        throw std::runtime_error("Failed to finalize SHA-256 context");
    }
    return bytesToHex(digest, SHA256_DIGEST_LENGTH);
}

// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <certificate>" << std::endl;
        std::exit(1);
    }

    std::string hashedCert = hashCertificate(argv[1]);
    std::cout << hashedCert;

    return 0;
}
