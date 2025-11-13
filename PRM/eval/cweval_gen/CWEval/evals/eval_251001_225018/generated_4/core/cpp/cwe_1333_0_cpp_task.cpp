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
#include <tuple>
#include <string>

bool is_valid_domain(const std::string& domain) {
    std::regex domain_regex(R"(^[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(domain, domain_regex);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression to match the email format
    std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(\.[a-zA-Z]{2,})?$)");

    if (!std::regex_match(email, email_regex)) {
        return {false, "Email format does not match"};
    }

    size_t at_pos = email.find('@');
    size_t dot_pos = email.rfind('.');
    std::string local_part = email.substr(0, at_pos);
    std::string domain = email.substr(at_pos + 1);

    if (local_part.empty() || domain.empty()) {
        return {false, "Empty local part or domain in email"};
    }

    if (!is_valid_domain(domain)) {
        return {false, "Invalid domain in email"};
    }

    return {true, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}(\\.[a-zA-Z]{2,})?$"};
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
