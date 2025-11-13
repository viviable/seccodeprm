```cpp
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string generate_receive_log(const std::string &msg) {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_c);

    // Format the timestamp as ISO 8601
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%Y-%m-%dT%H:%M:%S");

    // Create the log entry
    std::string log_entry = "[" + oss.str() + "] Received: " + msg;

    return log_entry;
}
```