```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path filePath(directory);
        if (!fs::exists(filePath)) {
            fs::create_directory(filePath);
        }
        filePath /= filename;

        // Open the file for writing with user-only permissions
        std::ofstream outFile(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());
        if (!outFile) {
            throw std::runtime_error("Failed to write to file");
        }

        // Set the file permissions to user-only read and write
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);

        outFile.close();
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << '\n';
    }
}
```