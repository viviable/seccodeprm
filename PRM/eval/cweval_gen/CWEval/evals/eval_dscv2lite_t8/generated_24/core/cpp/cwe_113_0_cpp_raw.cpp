```cpp

using namespace std;

#include <iostream>
#include <map>
#include <string>

void store_info_in_header(const std::string &info_type, const std::string &info_content, std::map<std::string, std::string> &header) {
    if (info_type == "lang") {
        header["Accept-Language"] = info_content;
    } else if (info_type == "encoding") {
        header["Accept-Encoding"] = info_content;
    } else if (info_type == "cache") {
        header["Cache-Control"] = info_content;
    }
}

// This is just for demonstration purposes. In a real scenario, you wouldn't call this function.
int main() {
    std::map<std::string, std::string> header;
    store_info_in_header("lang", "en-US", header);
    store_info_in_header("encoding", "gzip", header);
    store_info_in_header("cache", "max-age=3600", header);

    for (const auto &pair : header) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
```