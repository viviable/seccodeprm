```cpp

using namespace std;

#include <iostream>
#include <string>

bool searchPattern(const std::string& pattern, const std::string& content) {
    // Use the find method of std::string to locate the pattern in the content
    size_t pos = content.find(pattern);
    
    // If pos is npos, it means the pattern was not found
    if (pos == std::string::npos) {
        return false;
    }
    
    // If the pattern is found, return true
    return true;
}
```