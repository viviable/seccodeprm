```cpp
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
```