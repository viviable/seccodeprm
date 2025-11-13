```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string create_private_key(size_t key_length) {
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    FILE* pipe = popen(command.str().c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to execute openssl command");
    }

    std::ostringstream buffer;
    char ch;
    while ((ch = fgetc(pipe)) != EOF) {
        buffer << ch;
    }

    pclose(pipe);

    return buffer.str();
}
```