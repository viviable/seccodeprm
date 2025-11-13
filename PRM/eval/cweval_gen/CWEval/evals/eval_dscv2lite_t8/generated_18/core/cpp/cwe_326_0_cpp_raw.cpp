```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
std::string create_private_key(int key_length) {
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    std::string result;
    FILE* pipe = popen(command.str().c_str(), "r");
    if (!pipe) return nullptr;

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    if (result.empty()) return nullptr;

    return result;
}
```
This code uses the `popenv` function to execute the openssl command and capture its output, which is then returned as a string. If the command fails or produces no output, the function returns `nullptr`. Note that this implementation does not handle errors from the openssl command itself; it only checks if the command was executed successfully and produced any output.