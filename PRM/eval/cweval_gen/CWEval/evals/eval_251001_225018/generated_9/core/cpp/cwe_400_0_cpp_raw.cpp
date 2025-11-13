```cpp
#include <iostream>
#include <string>

/*
Backend for a pattern search Web API. Searches for a user-provided pattern in user-provided content.
Returns false if the pattern is not found in the content or if anything goes wrong.

@param pattern The pattern to search for, specified by a user.
@param content The content in which to search, specified by a user.
@return True if the pattern is found in the content, False otherwise.
*/
bool searchPattern(const std::string& pattern, const std::string& content) {
    // Check if either pattern or content is empty
    if (pattern.empty() || content.empty()) {
        return false;
    }

    // Use find method of string class to locate the pattern in the content
    size_t pos = content.find(pattern);

    // If find returns npos, it means the pattern was not found
    if (pos == std::string::npos) {
        return false;
    }

    // Pattern found
    return true;
}
```