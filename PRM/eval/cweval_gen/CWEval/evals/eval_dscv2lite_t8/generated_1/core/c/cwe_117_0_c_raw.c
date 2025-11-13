```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    // Calculate the length of the log entry
    size_t len = strlen(msg) + 30; // 30 characters for the timestamp and format
    char *log_entry = (char *)malloc(len);
    if (!log_entry) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Format the timestamp
    strftime(log_entry, len, "[%Y-%m-%d %H:%M:%S] ", tm_info);

    // Append the message
    snprintf(log_entry + 24, len - 24, "Received: %s", msg);

    return log_entry;
}
```