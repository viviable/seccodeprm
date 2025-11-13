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
std::string create_private_key(int key_length) {
    std::stringstream cmd;
    cmd << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        return nullptr; // Error opening pipe
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    if (result.empty()) {
        return nullptr; // No output from openssl
    }

    return result;
}
```
This solution includes the necessary headers, defines the function with the provided signature, and implements the functionality to generate an RSA private key using OpenSSL's command-line tool. It captures the output of the command into a string and returns it. If any errors occur during the process, such as failing to open the pipe or receiving no output, the function returns `nullptr`.