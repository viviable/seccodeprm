```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
std::string create_private_key(int key_length = 2048) {
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    try {
        std::ifstream pipe(command.str().c_str());
        if (!pipe.is_open()) {
            throw std::runtime_error("Failed to execute openssl command");
        }

        std::ostringstream buffer;
        buffer << pipe.rdbuf();
        if (!buffer) {
            throw std::runtime_error("Failed to read from openssl command output");
        }

        return buffer.str();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return nullptr;
    }
}
```