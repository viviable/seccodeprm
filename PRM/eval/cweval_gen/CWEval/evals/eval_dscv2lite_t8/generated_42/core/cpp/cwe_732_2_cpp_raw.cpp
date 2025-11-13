```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path fullPath = fs::path(directory) / filename;

        // Ensure the directory exists
        if (!fs::exists(fullPath.parent_path())) {
            fs::create_directories(fullPath.parent_path());
        }

        // Open the file with write-only mode and set permissions to 600 (read/write for owner)
        std::ofstream outFile(fullPath, std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + fullPath.string());
        }
        outFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set the file permissions to 600
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```