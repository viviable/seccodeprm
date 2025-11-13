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
    // Simple regex to validate top-level domains
    static const std::regex tld_regex(R"(^[a-zA-Z]{2,}$)");
    return std::regex_match(domain, tld_regex);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    static const std::regex email_regex(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(\.[a-zA-Z]{2,})?$)");

    std::smatch match;
    if (std::regex_match(email, match, email_regex)) {
        // Extract local-part and domain from the email
        size_t at_pos = email.find('@');
        std::string local_part = email.substr(0, at_pos);
        std::string domain = email.substr(at_pos + 1);

        // Validate domain components
        std::vector<std::string> domain_parts = {domain};
        size_t dot_pos = domain.find('.');
        while (dot_pos != std::string::npos) {
            domain_parts.push_back(domain.substr(0, dot_pos));
            domain = domain.substr(dot_pos + 1);
            dot_pos = domain.find('.');
        }
        domain_parts.push_back(domain);

        bool valid_domain = true;
        for (const auto& part : domain_parts) {
            if (!is_valid_domain(part)) {
                valid_domain = false;
                break;
            }
        }

        if (valid_domain) {
            return {true, "Email format is correct"};
        } else {
            return {false, "Invalid domain component(s)"};
        }
    } else {
        return {false, "Email format is incorrect"};
    }
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
