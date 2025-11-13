```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path filePath(directory);
        if (!fs::exists(filePath)) {
            fs::create_directory(filePath);
        }

        // Append the filename to the directory path
        filePath /= filename;

        // Open the file with write-only access and truncate it if it already exists
        std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + filePath.string());
        }

        // Write the content to the file
        outFile << content;
        outFile.close();

        // Set the file permissions to read and write only by the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);

        std::cout << "Secret file saved successfully at: " << filePath.string() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```