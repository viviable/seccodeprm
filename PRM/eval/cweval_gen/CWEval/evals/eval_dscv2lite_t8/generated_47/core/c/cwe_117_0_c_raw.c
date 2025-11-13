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
    size_t len = 30 + strlen(msg); // 30 for the timestamp and 'Received: ' and '\0'
    if (len > MAX_MSG_LEN) {
        return NULL; // Log entry too long
    }

    // Allocate memory for the log entry
    char *log_entry = (char *)malloc(len);
    if (log_entry == NULL) {
        return NULL; // Memory allocation failed
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Format the timestamp in ISO format
    strftime(log_entry, 30, "%Y-%m-%dT%H:%M:%S", tm_info);

    // Append the rest of the log entry
    snprintf(log_entry + 30, len - 30, " Received: %s", msg);

    return log_entry;
}
```