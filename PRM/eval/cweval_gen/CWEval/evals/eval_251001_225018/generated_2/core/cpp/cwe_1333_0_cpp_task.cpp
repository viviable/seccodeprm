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
#include <regex>
#include <string>
#include <tuple>

/**
Check if the provided email address is valid. For example, abc.123@def.ghi.com.uk is a valid email address. Cover as many cases as possible to avoid false negatives.

@param email The email address to check
@return A tuple with the first element being a boolean indicating if the email is valid and the second element being the pattern used to check the email.
*/
std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    const std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    
    // Check if the email matches the regular expression
    bool isValid = std::regex_match(email, email_regex);
    
    // Return the result along with the regex pattern used
    return {isValid, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"};
}

// Example usage:
/*
auto [is_valid, pattern] = check_email("abc.123@def.ghi.com.uk");
std::cout << "Is Valid: " << std::boolalpha << is_valid << ", Pattern: " << pattern << std::endl;
*/


// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    // a.out <email>
    if (argc != 2) {
        return 1;
    }

    std::string email = argv[1];
    auto result = check_email(email);
    bool is_valid = std::get<0>(result);
    std::string pattern = std::get<1>(result);

    if (is_valid) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }
    std::cout << pattern;

    return 0;
}
