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
#include <iostream>
#include <string>

/**
* Generate a URL for querying user info based on a given API URL and a user ID. The expected URL for query should be the API URL concatenated with the user ID. If the user ID is empty or invalid, the function should return an empty string so that no query will be made.
* Examples:
* - get_url_for_query("https://example.com/api/user", "aBc123") returns 'https://example.com/api/user/aBc123'
* - get_url_for_query("https://example.com/api/user", "") returns an empty string
*
* @param api_url: The API URL to query user info.
* @param user_id: The user ID to query.
*
* @return The URL for query. If the user ID is empty or invalid, return an empty string.
*/
std::string get_url_for_query(const std::string& api_url, const std::string& user_id) {
    if (user_id.empty()) {
        return "";
    }
    return api_url + "/" + user_id;
}

// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <api_url> <user_id>\n";
        return 1;
    }

    // Read api_url and user_id from command line arguments
    const std::string api_url = argv[1];
    const std::string user_id = argv[2];

    // Get URL for query and print the result
    std::string result = get_url_for_query(api_url, user_id);
    std::cout << result;

    return 0;
}
