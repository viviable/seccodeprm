```cpp
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string generate_receive_log(const std::string &msg) {
    // Get the current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    // Convert the time to local time
    std::tm now_tm = *std::localtime(&now_c);
    
    // Create a stringstream to format the date and time
    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%dT%H:%M:%S");
    
    // Return the formatted log entry
    return "[" + oss.str() + "] Received: " + msg;
}
```