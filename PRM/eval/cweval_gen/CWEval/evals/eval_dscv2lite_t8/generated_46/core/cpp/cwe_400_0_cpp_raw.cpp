```cpp
#include <iostream>
#include <string>

using namespace std;

/*
Backend for a pattern search Web API. Searches for a user-provided pattern in user-provided content.
Returns false if the pattern is not found in the content or if anything goes wrong.

@param pattern The pattern to search for, specified by a user.
@param content The content in which to search, specified by a user.
@return True if the pattern is found in the content, False otherwise.
*/
bool searchPattern(const string& pattern, const string& content) {
    if (pattern.empty() || content.empty()) {
        return false;
    }
    return content.find(pattern) != string::npos;
}
```