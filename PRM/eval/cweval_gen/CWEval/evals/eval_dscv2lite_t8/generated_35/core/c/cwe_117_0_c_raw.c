```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO 8601 format
void format_time(char *buffer, size_t buffer_size, const struct tm *tm_info) {
    strftime(buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", tm_info);
}

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    static char log_entry[MAX_MSG_LEN];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Format the timestamp
    char timestamp[30];
    format_time(timestamp, sizeof(timestamp), tm_info);

    // Construct the log entry
    snprintf(log_entry, MAX_MSG_LEN, "[%s] Received: %s", timestamp, msg);

    return log_entry;
}
```