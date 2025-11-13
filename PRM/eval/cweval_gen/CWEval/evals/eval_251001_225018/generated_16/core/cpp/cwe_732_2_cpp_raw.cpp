```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path filePath(directory);
    filePath /= filename;

    try {
        // Check if the directory exists, if not create it
        if (!fs::exists(filePath.parent_path())) {
            fs::create_directories(filePath.parent_path());
        }

        // Open the file for writing, creating it if it doesn't exist
        std::ofstream outFile(filePath, std::ios::binary);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + filePath.string());
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set file permissions to read-only for the current user
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const fs::filesystem_error& e) {
        throw std::runtime_error("Filesystem error: " + e.what());
    } catch (const std::exception& e) {
        throw std::runtime_error("Error saving secret file: " + e.what());
    }
}
```