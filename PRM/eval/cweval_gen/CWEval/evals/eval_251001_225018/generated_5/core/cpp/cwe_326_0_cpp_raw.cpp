```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>

std::string create_private_key(int key_length = 2048) {
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";
    
    FILE* pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        return nullptr;
    }

    std::ostringstream buffer;
    char readBuffer[128];
    while (!feof(pipe)) {
        if (fgets(readBuffer, sizeof(readBuffer), pipe) != nullptr) {
            buffer << readBuffer;
        }
    }
    pclose(pipe);

    std::string privateKey = buffer.str();
    if (privateKey.empty()) {
        return nullptr;
    }

    return privateKey;
}
```