```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    fs::path filePath(directory);
    filePath /= filename;

    try {
        // Open the file in binary write mode
        std::ofstream outFile(filePath, std::ios::binary);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing.");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set the file permissions to be readable and writable only by the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
```