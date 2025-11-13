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

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Choose AES-256-CBC as the encryption algorithm
    const int key_length = 32; // AES-256 requires a 32-byte key
    const int iv_length = AES_BLOCK_SIZE; // AES block size is 16 bytes

    unsigned char aes_key[key_length];
    memcpy(aes_key, key.data(), key_length);

    unsigned char iv[iv_length];
    if (!RAND_bytes(iv, iv_length)) {
        throw runtime_error("Failed to generate IV");
    }

    std::vector<unsigned char> padded_data = pad_data(data);
    std::vector<unsigned char> encrypted_data(padded_data.size());

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 8 * key_length, &enc_key) != 0) {
        throw runtime_error("Failed to set encryption key");
    }

    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.iv = iv;
    result.key.assign(aes_key, aes_key + key_length);

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
