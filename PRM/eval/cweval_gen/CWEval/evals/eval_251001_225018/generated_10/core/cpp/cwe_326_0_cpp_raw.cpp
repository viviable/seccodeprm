```cpp
#include <iostream>
#include <string>
#include <cstdio>

std::string create_private_key(int key_length = 2048) {
    char command[1024];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    FILE* pipe = popen(command, "r");
    if (!pipe) {
        std::cerr << "Failed to execute command: " << command << std::endl;
        return nullptr;
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    return result;
}
```