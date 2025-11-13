```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>

std::string create_private_key() {
    const int key_length = 2048; // Example key length, can be adjusted
    std::stringstream cmd;
    cmd << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute command: " << cmd.str() << std::endl;
        return nullptr;
    }

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }
    pclose(pipe);

    if (result.empty()) {
        std::cerr << "No output from openssl command" << std::endl;
        return nullptr;
    }

    return result;
}
```