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
    std::time_t now = std::time(nullptr);
    std::tm localTime;
    localtime_s(&localTime, &now); // Safe version of localtime

    std::ostringstream oss;
    oss << "[" << std::put_time(&localTime, "%Y-%m-%dT%H:%M:%S") << "] Received: " << msg;

    return oss.str();
}
```