```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path filePath(directory);
        filePath /= filename;

        // Ensure the directory exists
        if (!fs::exists(filePath.parent_path())) {
            fs::create_directories(filePath.parent_path());
        }

        // Open the file with write-only mode and restrict access to the current user
        std::ofstream outFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Set the file permissions to read and write only for the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        if (!outFile) {
            throw std::runtime_error("Failed to write to file");
        }

        outFile.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```