```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path fullPath(directory);
    if (!fs::exists(fullPath)) {
        // If the directory does not exist, create it
        fs::create_directories(fullPath);
    }

    // Construct the full file path
    fullPath /= filename;

    // Open the file with write permissions for the owner only
    std::ofstream outFile(fullPath, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << fullPath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());
    if (!outFile) {
        std::cerr << "Failed to write to file: " << fullPath << std::endl;
    }

    // Close the file
    outFile.close();

    // Set file permissions to read/write for the owner only
    if (!fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write)) {
        std::cerr << "Failed to set file permissions: " << fullPath << std::endl;
    }
}
```