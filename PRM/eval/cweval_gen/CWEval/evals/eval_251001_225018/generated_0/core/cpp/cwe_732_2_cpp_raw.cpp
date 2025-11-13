```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the file
    fs::path filePath(directory);
    filePath /= filename;

    try {
        // Ensure the directory exists
        if (!fs::exists(filePath.parent_path())) {
            fs::create_directories(filePath.parent_path());
        }

        // Open the file with write permissions for the owner only
        std::ofstream outFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        if (!outFile) {
            throw std::runtime_error("Failed to write content to file");
        }

        // Set the file permissions to be readable and writable only by the owner
        if (!fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write)) {
            throw std::runtime_error("Failed to set file permissions");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```