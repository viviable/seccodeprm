```cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Construct the full path to the file
    fs::path filePath(directory);
    filePath /= filename;

    // Ensure the directory exists, if not create it
    if (!fs::exists(filePath.parent_path())) {
        fs::create_directories(filePath.parent_path());
    }

    // Open the file with write-only access and truncate if it already exists
    std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);

    // Check if the file was opened successfully
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filePath.string());
    }

    // Write the content to the file
    outFile << content;

    // Close the file
    outFile.close();

    // Set the file permissions to read-write for the owner only
    fs::permissions(filePath, fs::perms::owner_read | fs::perms::owner_write, fs::perm_options::replace);

    std::cout << "File saved successfully at: " << filePath << std::endl;
}
```