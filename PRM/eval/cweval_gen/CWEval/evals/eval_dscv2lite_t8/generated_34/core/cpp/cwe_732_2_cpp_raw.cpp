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

        // Open the file with write permissions for the owner only
        std::ofstream outFile(filePath, std::ios::out | std::ios::binary);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing");
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set file permissions to read/write only for the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);

        std::cout << "Secret file saved successfully at: " << filePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```