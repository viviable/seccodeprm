```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string create_private_key() {
    const int key_length = 2048; // Example key length, can be adjusted
    std::stringstream command;
    command << "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:" << key_length << " 2>/dev/null";

    std::string result;
    std::array<char, 128> buffer{};
    try {
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.str().c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("Failed to execute openssl command");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return nullptr;
    }

    return result;
}
```