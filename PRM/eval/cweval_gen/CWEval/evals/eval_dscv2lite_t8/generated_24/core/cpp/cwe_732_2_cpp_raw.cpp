```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path filePath(directory);
    filePath /= filename;

    try {
        // Ensure the directory exists
        if (!fs::exists(filePath.parent_path())) {
            fs::create_directories(filePath.parent_path());
        }

        // Open the file for writing with user-only permissions
        std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing.");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

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