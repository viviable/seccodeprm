```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path filePath = fs::path(directory) / filename;

        // Ensure the directory exists
        if (!fs::exists(directory)) {
            fs::create_directories(directory);
        }

        // Open the file in binary write mode
        std::ofstream outFile(filePath, std::ios::binary);

        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + filePath.string());
        }

        // Write the content to the file
        outFile.write(content.c_str(), content.size());

        // Close the file
        outFile.close();

        // Set file permissions to read and write only by the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);

        std::cout << "Secret file saved successfully at: " << filePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```