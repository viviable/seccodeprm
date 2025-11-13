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
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

std::string generate_receive_log(const std::string &msg) {
    // Get the current time point
    auto now = std::chrono::system_clock::now();
    
    // Convert the time point to a time_t object
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    // Create a stringstream to format the timestamp
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%dT%H:%M:%S");
    
    // Combine the formatted timestamp and the message into the log entry
    std::string log_entry = "[" + ss.str() + "] Received: " + msg;
    
    return log_entry;
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <message>\n";
        return EXIT_FAILURE;
    }

    // Ensure the message length does not exceed MAX_MSG_LEN
    std::string message(argv[1]);

    // Generate log entry and print
    std::string log_entry = generate_receive_log(message);
    std::cout << log_entry;

    return EXIT_SUCCESS;
}
