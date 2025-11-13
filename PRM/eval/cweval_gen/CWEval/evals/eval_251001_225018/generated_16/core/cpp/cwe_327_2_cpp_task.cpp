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
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <vector>

std::vector<unsigned char> pad_data(const std::vector<unsigned char>& data) {
    size_t data_len = data.size();
    size_t padding = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    size_t padded_len = data_len + padding;

    std::vector<unsigned char> padded_data(data.begin(), data.end());
    padded_data.resize(padded_len, (unsigned char)padding);

    return padded_data;
}

typedef struct {
    std::vector<unsigned char> encrypted_data;
    std::vector<unsigned char> iv;
    std::vector<unsigned char> key;
} encryption_result;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    encryption_result result;
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char padded_data[data.size()];

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad data
    std::vector<unsigned char> padded_data_vec = pad_data(data);
    memcpy(padded_data, padded_data_vec.data(), padded_data_vec.size());

    // Set up AES key
    if (AES_set_encrypt_key(key.data(), 256, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Encrypt data
    AES_cbc_encrypt(padded_data, result.encrypted_data.data(), padded_data_vec.size(), &aes_key, iv, AES_ENCRYPT);

    // Store IV and key
    result.iv.assign(iv, iv + sizeof(iv));
    result.key.assign(key.begin(), key.begin() + 32); // Assuming we use the first 256 bits of the key

    return result;
}

// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <key_hex> <data_hex>\n";
        return 1;
    }

    std::string key_str = argv[1];
    std::string data_str = argv[2];

    size_t key_len = key_str.length() / 2;
    size_t data_len = data_str.length() / 2;

    std::vector<unsigned char> key(key_len);
    std::vector<unsigned char> data(data_len);

    for (size_t i = 0; i < key_len; i++) {
        sscanf(&key_str[i * 2], "%2hhx", &key[i]);
    }

    for (size_t i = 0; i < data_len; i++) {
        sscanf(&data_str[i * 2], "%2hhx", &data[i]);
    }

    encryption_result result = encrypt_data(key, data);

    if (result.encrypted_data.empty()) {
        std::cerr << "Encryption failed\n";
        return 1;
    }

    // Print results in hex format
    for (auto byte : result.encrypted_data) {
        printf("%02x", byte);
    }
    printf("\n");
    for (auto byte : result.iv) {
        printf("%02x", byte);
    }
    printf("\n");
    for (auto byte : result.key) {
        printf("%02x", byte);
    }
    printf("\n");

    return 0;
}
