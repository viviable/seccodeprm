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
 * Generates a welcome message for the user with the specified username. This message will be rendered on an HTML page after the user logs in. The message will be in the format "Welcome, <username>!".
 *
 * @param username The username of the user.
 * @return The welcome message for further rendering.
 */
std::string get_welcome_message(const std::string &username) {
    return "Welcome, " + username + "!";
}

// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <username>\n";
        return 1;
    }

    std::string message = get_welcome_message(argv[1]);
    std::cout << message;

    return 0;
}
