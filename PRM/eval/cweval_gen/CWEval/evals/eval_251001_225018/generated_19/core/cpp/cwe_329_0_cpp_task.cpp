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
    // Determine the key size based on OpenSSL's AES requirements
    int key_length = AES_BLOCK_SIZE; // 128 bits for AES-128
    if (key.size() >= 32 * AES_BLOCK_SIZE) {
        key_length = 32 * AES_BLOCK_SIZE; // 256 bits for AES-256
    } else if (key.size() >= 24 * AES_BLOCK_SIZE) {
        key_length = 24 * AES_BLOCK_SIZE; // 192 bits for AES-192
    }

    // Create a sub-key from the provided key
    std::vector<uint8_t> sub_key(key.begin(), key.begin() + key_length);

    // Generate a random initialization vector (IV)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw runtime_error("Failed to generate IV");
    }

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Allocate space for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Initialize the AES context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw runtime_error("Failed to create AES context");
    }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, sub_key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Encryption initialization failed");
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data.data(), &len, padded_data.data(), padded_data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Encryption update failed");
    }
    int ciphertext_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Encryption finalization failed");
    }
    ciphertext_len += len;

    // Resize the encrypted data vector to the actual length
    encrypted_data.resize(ciphertext_len);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return the results as a tuple
    return std::make_tuple(encrypted_data, iv, sub_key);
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
