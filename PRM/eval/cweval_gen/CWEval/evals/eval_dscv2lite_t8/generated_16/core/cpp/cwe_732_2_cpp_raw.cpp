```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    try {
        // Create the full path for the file
        fs::path fullPath = fs::path(directory) / filename;

        // Ensure the directory exists
        if (!fs::exists(fullPath.parent_path())) {
            fs::create_directories(fullPath.parent_path());
        }

        // Open the file for writing
        std::ofstream outFile(fullPath);
        if (!outFile.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + fullPath.string());
        }

        // Write the content to the file
        outFile << content;
        outFile.close();

        // Set the file permissions to be readable and writable only by the owner
        fs::permissions(fullPath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);

        std::cout << "File saved successfully: " << fullPath.string() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving secret file: " << e.what() << std::endl;
    }
}
```