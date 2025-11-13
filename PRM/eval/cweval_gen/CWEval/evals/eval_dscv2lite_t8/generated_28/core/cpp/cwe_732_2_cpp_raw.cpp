```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        std::string fullPath = directory + "/" + filename;

        // Ensure the directory exists
        if (!fs::exists(directory)) {
            fs::create_directories(directory);
        }

        // Open the file with write-only access and truncate if it already exists
        std::ofstream outFile(fullPath, std::ios::out | std::ios::trunc);

        // Check if the file was opened successfully
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file: " + fullPath);
        }

        // Write the content to the file
        outFile << content;

        // Close the file
        outFile.close();

        // Set the file permissions to read-write for the owner only
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);

        std::cout << "Secret file saved successfully: " << fullPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```