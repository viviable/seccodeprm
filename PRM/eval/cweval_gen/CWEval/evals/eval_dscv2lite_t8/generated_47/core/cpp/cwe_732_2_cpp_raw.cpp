```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path fullPath = fs::path(directory) / filename;

    // Ensure the directory exists, create if not
    if (!fs::exists(fullPath.parent_path())) {
        fs::create_directories(fullPath.parent_path());
    }

    try {
        // Open the file with write-only access and set permissions to read-write for the owner only
        std::ofstream outFile(fullPath, std::ios::out | std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }
        
        // Set file permissions to 600 (read-write for the owner)
        if (!fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write)) {
            throw std::runtime_error("Failed to set file permissions");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        if (!outFile) {
            throw std::runtime_error("Failed to write content to file");
        }

        // Close the file
        outFile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```