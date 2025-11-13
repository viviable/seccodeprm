```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path fullPath = fs::path(directory) / filename;

        // Ensure the directory exists
        if (!fs::exists(fullPath.parent_path())) {
            fs::create_directories(fullPath.parent_path());
        }

        // Open the file with write-only access and set permissions to read/write for the owner only
        std::ofstream outFile(fullPath, std::ios::out | std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }
        outFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Set the file permissions to be readable/writable only by the owner
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write);

        // Close the file
        outFile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```