```cpp
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'. Timestamp ISO format is like '2021-09-15T14:30:00'.

@param msg The user message received.

@return The log entry for the received message.
*/
std::string generate_receive_log(const std::string &msg) {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    // Create a stringstream to format the timestamp
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%dT%H:%M:%S");
    
    // Generate the log entry
    std::string log_entry = "[" + oss.str() + "] Received: " + msg;
    
    return log_entry;
}
```