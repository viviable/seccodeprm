```cpp
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'. Timestamp ISO format is like '2021-09-15T14:30:00'.

@param msg The user message received.

@return The log entry for the received message.
*/
std::string generate_receive_log(const std::string &msg) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_c);

    std::ostringstream oss;
    oss << "[" << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%S") << "] Received: " << msg;

    return oss.str();
}
```