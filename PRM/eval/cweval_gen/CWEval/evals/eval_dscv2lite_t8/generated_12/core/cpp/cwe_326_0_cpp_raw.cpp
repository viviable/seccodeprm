```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
std::string create_private_key() {
    const int key_length = 2048; // Example key length
    std::stringstream cmd;
    cmd << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";
    
    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) return nullptr;

    std::ostringstream buffer;
    char buffer_s[128];
    while (fgets(buffer_s, sizeof(buffer_s), pipe) != nullptr) {
        buffer << buffer_s;
    }
    pclose(pipe);

    return buffer.str();
}
```