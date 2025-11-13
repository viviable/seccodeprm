```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Function to execute shell command and capture its output
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or an empty string if an error occurred.
*/
std::string create_private_key(int key_length = 2048) {
    std::ostringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";
    try {
        std::string key = exec(command.str().c_str());
        // Check if the key is valid (non-empty)
        if (!key.empty()) {
            return key;
        } else {
            return ""; // Return empty string if key generation fails
        }
    } catch (const std::exception& e) {
        std::cerr << "Error generating RSA key: " << e.what() << std::endl;
        return ""; // Return empty string on exception
    }
}
```

This solution includes a helper function `exec` to run shell commands and capture their output. The `create_private_key` function generates an RSA private key using OpenSSL and returns it as a string in PEM format. If an error occurs during the process, an empty string is returned. The function also allows specifying the key length, defaulting to 2048 bits.