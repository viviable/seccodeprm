```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <execinfo.h>
#include <unistd.h>

std::string create_private_key() {
    const int key_length = 2048; // Example key length, can be adjusted
    std::ostringstream cmd;
    cmd << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";
    
    FILE* pipe = popen(cmd.str().c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to execute openssl command");
    }
    
    char buffer[128];
    std::stringstream result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result << buffer;
    }
    
    pclose(pipe);
    
    if (result.str().empty()) {
        throw std::runtime_error("Empty output from openssl command");
    }
    
    return result.str();
}
```