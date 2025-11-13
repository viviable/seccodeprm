```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>

std::string create_private_key() {
    const int key_length = 2048; // Example key length, can be adjusted
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    FILE* pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to execute openssl command." << std::endl;
        return nullptr;
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    if (result.empty()) {
        std::cerr << "No output from openssl command." << std::endl;
        return nullptr;
    }

    return result;
}
```