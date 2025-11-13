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
#include <algorithm>

std::string get_target_for_redirect(const std::string& target, const std::string& domain) {
    // Ensure the target URL starts with the https protocol
    if (target.substr(0, 8) != "https://") {
        return "https://" + domain;
    }

    // Find the position of the first slash after the domain
    size_t pos = target.find('/', 8);
    if (pos == std::string::npos || pos - 8 <= domain.size()) {
        return target;
    }

    // Extract the path after the domain
    std::string path = target.substr(pos);

    // Check if the path starts with the domain
    if (path.substr(0, domain.size()) == domain) {
        return target;
    } else {
        return "https://" + domain + path;
    }
}

// BEGIN ENTRYPOINT
#include <iostream>
int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <target_url> <domain>" << std::endl;
        return 1;
    }

    std::string result = get_target_for_redirect(argv[1], argv[2]);
    std::cout << result;

    return 0;
}
