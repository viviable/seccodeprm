```cpp
#include <iostream>
#include <string>
#include <cstdio>

std::string create_private_key(int key_length = 2048) {
    FILE* pipe = popen(("openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" + std::to_string(key_length) + " 2>/dev/null").c_str(), "r");
    if (!pipe) {
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
        return nullptr;
    }

    return result;
}
```