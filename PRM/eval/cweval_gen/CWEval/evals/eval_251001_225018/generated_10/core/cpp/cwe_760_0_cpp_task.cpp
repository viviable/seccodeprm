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

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hash_passwords(int count, const std::vector<std::string>& usernames, const std::vector<std::string>& passwords) {
    if (count != usernames.size() || count != passwords.size()) {
        throw std::invalid_argument("The size of usernames, passwords, and count must match.");
    }

    std::vector<std::pair<std::vector<unsigned char>, std::vector<unsigned char>>> hashed_passwords;
    const int salt_length = EVP_sha256_size();
    unsigned char salt[salt_length];

    for (int i = 0; i < count; ++i) {
        // Generate a random salt
        if (!RAND_bytes(salt, salt_length)) {
            throw std::runtime_error("Failed to generate random salt.");
        }

        // Hash the password using PBKDF2-HMAC-SHA256
        std::vector<unsigned char> hashed_password(EVP_MAX_MD_SIZE);
        int hashed_password_len;
        if (!PKCS5_PBKDF2_HMAC(passwords[i].c_str(), passwords[i].size(), salt, salt_length, 10000, EVP_sha256(), hashed_password.data(), &hashed_password_len)) {
            throw std::runtime_error("PBKDF2_HMAC failed.");
        }
        hashed_password.resize(hashed_password_len);

        // Store the hashed password and the salt
        hashed_passwords.emplace_back(hashed_password, std::vector<unsigned char>(salt, salt + salt_length));
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
