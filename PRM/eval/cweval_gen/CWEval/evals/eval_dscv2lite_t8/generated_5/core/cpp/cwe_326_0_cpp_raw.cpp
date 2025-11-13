```cpp
#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
std::string create_private_key(int key_length) {
    char command[1024];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    FILE* pipe = popen(command, "r");
    if (!pipe) {
        std::cerr << "Failed to execute command: " << command << std::endl;
        return nullptr;
    }

    char buffer[1024];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    if (result.empty()) {
        std::cerr << "No output from command: " << command << std::endl;
        return nullptr;
    }

    return result;
}
```