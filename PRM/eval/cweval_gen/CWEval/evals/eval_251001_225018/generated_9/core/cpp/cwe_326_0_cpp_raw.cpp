```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
std::string create_private_key() {
    const int key_length = 2048; // Example key length
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";
    
    FILE* pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute command: " << command.str() << std::endl;
        return nullptr;
    }

    std::ostringstream buffer;
    char buffer_array[128];
    while (fgets(buffer_array, sizeof(buffer_array), pipe) != nullptr) {
        buffer << buffer_array;
    }
    pclose(pipe);

    return buffer.str();
}
```