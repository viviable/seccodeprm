```cpp
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

std::string create_private_key() {
    const int key_length = 2048; // Default RSA key length
    std::array<char, 1024 * 16> buffer{}; // Buffer to store the generated key
    FILE* pipe = popen(("openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" + std::to_string(key_length) + " 2>/dev/null").c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute openssl command." << std::endl;
        return nullptr;
    }
    size_t bytesRead = fread(buffer.data(), 1, sizeof(buffer), pipe);
    pclose(pipe);
    if (bytesRead == 0) {
        std::cerr << "No data received from openssl command." << std::endl;
        return nullptr;
    }
    return std::string(buffer.data(), bytesRead);
}
```