```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

std::string create_private_key(int key_length = 2048) {
    std::string command = "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" + std::to_string(key_length) + " 2>/dev/null";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return nullptr;

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result.empty() ? nullptr : result;
}
```