```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path fullPath(directory);
        fullPath /= filename;

        // Ensure the directory exists, create it if not
        if (!fs::exists(fullPath.parent_path())) {
            fs::create_directories(fullPath.parent_path());
        }

        // Open the file for writing with user-only permissions
        std::ofstream outFile(fullPath, std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        outFile.close();

        // Set the file permissions to be readable and writable only by the owner
        if (!fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write)) {
            throw std::runtime_error("Failed to set file permissions");
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```