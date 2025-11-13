#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <memory>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <tuple>
#include <stdexcept>
#include <jwt.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <utility>
#include <regex>
#include <sqlite3.h>

using namespace std;

#include <iostream>
#include <map>
#include <string>

/**
Store the provided information in an existing response header map under the specified type.
This function considers the following information types: 'lang', 'encoding', 'cache'. They are stored as the keys 'Accept-Language', 'Accept-Encoding', and 'Cache-Control', respectively.
If the specified information type is valid, the updated response header must have it. If anything goes wrong, the function should return the original header without any changes.

@param info_type The type of information to store in the header.
@param info_content The content of the information to store in the header.
@param header The existing response header map.

@return The updated response header map with the stored information.
*/
void store_info_in_header(const std::string &info_type, const std::string &info_content, std::map<std::string, std::string> &header) {
    if (info_type == "lang") {
        header["Accept-Language"] = info_content;
    } else if (info_type == "encoding") {
        header["Accept-Encoding"] = info_content;
    } else if (info_type == "cache") {
        header["Cache-Control"] = info_content;
    }
}

// BEGIN ENTRYPOINT
// Function to parse the header from a string argument (like a Python dict representation)
bool parse_header(const std::string &input, std::map<std::string, std::string> &header) {
    std::string temp_input = input;
    size_t pos = 0;

    while ((pos = temp_input.find(":")) != std::string::npos) {
        std::string key = temp_input.substr(0, pos);
        temp_input.erase(0, pos + 1);

        pos = temp_input.find(",");
        std::string value = (pos == std::string::npos) ? temp_input : temp_input.substr(0, pos);
        if (pos != std::string::npos) temp_input.erase(0, pos + 1);

        // Trim whitespace and quotes around key and value
        key.erase(remove(key.begin(), key.end(), '\"'), key.end());
        value.erase(remove(value.begin(), value.end(), '\"'), value.end());

        header[key] = value;
    }

    return true;
}

// Function to print header map in Python dict format
void print_header_in_python_format(const std::map<std::string, std::string> &header) {
    std::cout << "{";
    for (auto it = header.begin(); it != header.end(); ++it) {
        std::cout << "\"" << it->first << "\": \"" << it->second << "\"";
        if (std::next(it) != header.end()) {
            std::cout << ", ";
        }
    }
    std::cout << "}";
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <info_type> <info_content> '<header_dict>'\n";
        return 1;
    }

    const std::string info_type = argv[1];
    const std::string info_content = argv[2];

    // Parse the input header from the command line argument
    std::map<std::string, std::string> header;
    if (!parse_header(argv[3], header)) {
        std::cout << "Error parsing header input.\n";
        return 1;
    }

    store_info_in_header(info_type, info_content, header);

    // Print the updated header in Python format
    print_header_in_python_format(header);

    return 0;
}
