```cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string create_private_key(size_t key_length = 2048) {
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";
    
    try {
        std::FILE* pipe = popen(command.str().c_str(), "r");
        if (!pipe) throw std::runtime_error("popen() failed!");
        
        std::ostringstream buffer;
        char readBuffer[128];
        while (!feof(pipe)) {
            if (fgets(readBuffer, sizeof(readBuffer), pipe) != nullptr)
                buffer << readBuffer;
        }
        pclose(pipe);
        
        return buffer.str();
    } catch (...) {
        return nullptr;
    }
}
```