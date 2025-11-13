```cpp

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
std::string create_private_key() {
    const int key_length = 2048; // Example key length
    std::array<char, 1024> buffer;
    FILE* pipe = popen(("openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" + std::to_string(key_length) + " 2>/dev/null").c_str(), "r");
    if (!pipe) {
        return nullptr;
    }
    std::string result = "";
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    return result.empty() ? nullptr : result;
}
```