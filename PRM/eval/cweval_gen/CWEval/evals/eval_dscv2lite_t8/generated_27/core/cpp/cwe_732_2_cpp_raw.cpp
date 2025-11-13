```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    std::string fullPath = directory + "/" + filename;

    // Check if the directory exists, if not create it
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    // Open the file with write-only access and fail if it already exists
    std::ofstream outFile(fullPath, std::ios::out | std::ios::binary);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + fullPath);
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());
    if (!outFile) {
        throw std::runtime_error("Failed to write to file: " + fullPath);
    }

    // Close the file
    outFile.close();

    // Set the file permissions to read and write only for the owner
    if (!fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write)) {
        throw std::runtime_error("Failed to set file permissions for: " + fullPath);
    }
}
```