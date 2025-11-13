```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path filePath(directory);
        if (!fs::exists(filePath)) {
            fs::create_directories(filePath);
        }

        filePath /= filename;

        // Open the file in binary write mode and set permissions to 600 (read/write for owner)
        std::ofstream outFile(filePath, std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        outFile.close();

        // Set file permissions to 600 (read/write for owner)
        if (!fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write)) {
            throw std::runtime_error("Failed to set file permissions");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```