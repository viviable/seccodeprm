```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path filePath = fs::path(directory) / filename;

        // Ensure the directory exists
        if (!fs::exists(directory)) {
            fs::create_directories(directory);
        }

        // Open the file with appropriate permissions
        std::ofstream outFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing.");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        if (!outFile) {
            throw std::runtime_error("Failed to write content to file.");
        }

        // Set the file permissions to be readable and writable only by the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);

        // Close the file
        outFile.close();
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
}
```