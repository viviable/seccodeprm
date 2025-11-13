```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path fullPath = fs::path(directory) / filename;

        // Ensure the directory exists, create it if not
        if (!fs::exists(fullPath.parent_path())) {
            fs::create_directories(fullPath.parent_path());
        }

        // Open the file in binary mode for writing
        std::ofstream outFile(fullPath, std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + fullPath.string());
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set file permissions to read-only for the owner
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }
}
```