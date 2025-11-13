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

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>
#include <tuple>

// Helper function to pad data according to PKCS7
std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_len = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), padding_len, static_cast<uint8_t>(padding_len));
    return padded;
}

/**
Encrypt the data with the key using AES and CBC mode.
The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
Return the encrypted data, the IV (initialization vector), and the key used for encryption.

@param key The bytes to generate the encryption key
@param data The data to encrypt
@return tuple containing: {encrypted data, IV, key used for encryption}
*/
std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Determine the effective key length (128, 192, or 256 bits)
    size_t key_length = key.size();
    if (key_length > 32) key_length = 32; // 256 bits
    else if (key_length > 24) key_length = 24; // 192 bits
    else if (key_length > 16) key_length = 16; // 128 bits

    // Create an AES key schedule from the provided key
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), key_length * 8, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    // Generate a random initialization vector
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), AES_BLOCK_SIZE)) {
        throw runtime_error("Failed to generate IV");
    }

    // Pad the data according to PKCS7
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Encrypt the data
    std::vector<uint8_t> encrypted_data(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    return make_tuple(encrypted_data, iv, vector<uint8_t>(key.begin(), key.begin() + key_length));
}

// BEGIN ENTRYPOINT
// Helper function to convert hex string to bytes
std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        bytes.push_back(static_cast<uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    }
    return bytes;
}

// Helper function to convert bytes to hex string
std::string bytes_to_hex(const std::vector<uint8_t>& bytes) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (const auto& byte : bytes) {
        ss << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <hex-key> <hex-data>\n";
            return 1;
        }

        // Convert hex strings to bytes
        std::vector<uint8_t> key = hex_to_bytes(argv[1]);
        std::vector<uint8_t> data = hex_to_bytes(argv[2]);

        // Perform encryption
        auto [encrypted_data, iv, used_key] = encrypt_data(key, data);

        // Print results in hex format
        // std::cout << "Encrypted data: " << bytes_to_hex(encrypted_data) << "\n";
        // std::cout << "IV: " << bytes_to_hex(iv) << "\n";
        // std::cout << "Used key: " << bytes_to_hex(used_key) << "\n";
        std::cout << bytes_to_hex(encrypted_data) << "\n";
        std::cout << bytes_to_hex(iv) << "\n";
        std::cout << bytes_to_hex(used_key) << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
