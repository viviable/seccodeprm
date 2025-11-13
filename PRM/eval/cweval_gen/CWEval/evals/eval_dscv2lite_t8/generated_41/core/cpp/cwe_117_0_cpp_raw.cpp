```cpp

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string generate_receive_log(const std::string &msg) {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    // Convert the time to a string in ISO 8601 format
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%dT%H:%M:%S");
    std::string timestamp = ss.str();
    
    // Create the log entry
    std::string log_entry = "[" + timestamp + "] Received: " + msg;
    return log_entry;
}
```