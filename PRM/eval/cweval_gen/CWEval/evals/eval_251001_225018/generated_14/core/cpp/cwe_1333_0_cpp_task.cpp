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

bool is_valid_domain(const std::string& domain) {
    // Check for at least one dot and no leading/trailing dots
    return domain.find('.') != std::string::npos && domain.front() != '.' && domain.back() != '.';
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    std::string pattern = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
    
    // Check for multiple consecutive dots or dots at the start/end of local part
    if (email.find("..") != std::string::npos || email.front() == '.' || email.back() == '.') {
        return {false, pattern};
    }
    
    size_t at_pos = email.find('@');
    if (at_pos == std::string::npos) {
        return {false, pattern};
    }
    
    std::string local_part = email.substr(0, at_pos);
    std::string domain = email.substr(at_pos + 1);
    
    // Check local part length and characters
    if (local_part.length() > 64 || local_part.empty()) {
        return {false, pattern};
    }
    for (char c : local_part) {
        if (!isalnum(c) && c != '.' && c != '_' && c != '%' && c != '+' && c != '-') {
            return {false, pattern};
        }
    }
    
    // Check domain length and validity
    if (domain.length() > 255) {
        return {false, pattern};
    }
    if (!is_valid_domain(domain)) {
        return {false, pattern};
    }
    
    // Regular expression check
    std::regex re(pattern);
    if (!std::regex_match(email, re)) {
        return {false, pattern};
    }
    
    return {true, pattern};
}

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
