```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void saveSecretFile(const std::string& directory, const std::string& filename, const std::string& content) {
    // Create the full path for the secret file
    std::string fullPath = directory + "/" + filename;

    // Check if the directory exists, if not create it
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    // Open the file with write permissions for the current user only
    std::ofstream outFile(fullPath, std::ios::out | std::ios::binary);

    // Check if the file was opened successfully
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << fullPath << std::endl;
        return;
    }

    // Write the content to the file
    outFile.write(content.c_str(), content.size());

    // Close the file
    outFile.close();

    std::cout << "Secret file saved successfully at: " << fullPath << std::endl;
}
```