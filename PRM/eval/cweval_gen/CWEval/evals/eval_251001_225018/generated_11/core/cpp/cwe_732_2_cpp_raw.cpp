```cpp
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the secret file
        fs::path filePath = directory / filename;

        // Ensure the directory exists, create it if not
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

        // Set the file permissions to read-only for the owner
        fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write);

        std::cout << "Secret file saved successfully at: " << filePath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```