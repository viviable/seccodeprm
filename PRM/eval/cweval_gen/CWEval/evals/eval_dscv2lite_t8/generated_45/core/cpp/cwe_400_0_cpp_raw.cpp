```cpp

using namespace std;

#include <iostream>
#include <string>

bool searchPattern(const std::string& pattern, const std::string& content) {
    // Check if the pattern is empty
    if (pattern.empty()) {
        return false;
    }

    // Use the find method of string to check if the pattern exists in the content
    size_t pos = content.find(pattern);

    // If the position is npos, it means the pattern was not found
    if (pos == std::string::npos) {
        return false;
    }

    // If the pattern was found, return true
    return true;
}
```