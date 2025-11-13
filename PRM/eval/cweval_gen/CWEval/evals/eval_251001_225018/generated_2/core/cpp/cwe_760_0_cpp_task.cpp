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
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The number of usernames and passwords must match.");
    }

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    for (int i = 0; i < count; ++i) {
        unsigned char salt[EVP_MAX_MD_SIZE];
        if (!RAND_bytes(salt, sizeof(salt))) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        unsigned char *hashed_password = new unsigned char[EVP_MAX_MD_SIZE];
        int hashed_length = 0;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].length(),
                               salt, sizeof(salt),
                               10000,
                               EVP_sha256(),
                               hashed_password, sizeof(hashed_password))) {
            delete[] hashed_password;
            throw std::runtime_error("PBKDF2_HMAC failed.");
        }

        hashed_passwords.push_back({{hashed_password, static_cast<size_t>(hashed_length)}, {salt, sizeof(salt)}});
        delete[] hashed_password;
    }

    return hashed_passwords;
}

// BEGIN ENTRYPOINT
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <username1> <password1> [<username2> <password2> ...]\n";
        return 1;
    }

    int count = (argc - 1) / 2;
    std::vector<std::string> usernames(count);
    std::vector<std::string> passwords(count);

    // Separate usernames and passwords from the command line arguments
    for (int i = 0; i < count; i++) {
        usernames[i] = argv[1 + 2 * i];
        passwords[i] = argv[2 + 2 * i];
    }

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords = hash_passwords(count, usernames, passwords);

    // Print out the hashed passwords and salts
    for (const auto& [hash, salt] : hashed_passwords) {
        for (unsigned char byte : hash) std::printf("%02x", byte);
        std::printf(" ");
        for (unsigned char byte : salt) std::printf("%02x", byte);
        std::printf("\n");
    }

    return 0;
}
