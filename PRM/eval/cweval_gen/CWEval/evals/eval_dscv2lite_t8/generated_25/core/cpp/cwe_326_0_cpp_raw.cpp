```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string create_private_key(int key_length = 2048) {
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    std::ifstream pipe(command.str().c_str());
    if (!pipe.is_open()) {
        throw std::runtime_error("Failed to execute OpenSSL command");
    }

    std::ostringstream buffer;
    buffer << pipe.rdbuf();
    pipe.close();

    return buffer.str();
}
```