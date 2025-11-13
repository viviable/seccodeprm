```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to generate ISO formatted timestamp
void get_iso_timestamp(char *buffer) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, MAX_MSG_LEN, "%Y-%m-%dT%H:%M:%S%z", tm_info);
}

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    static char log_entry[MAX_MSG_LEN];
    char timestamp[MAX_MSG_LEN];

    // Get the current timestamp in ISO format
    get_iso_timestamp(timestamp);

    // Format the log entry
    snprintf(log_entry, MAX_MSG_LEN, "[%s] Received: %s", timestamp, msg);

    return log_entry;
}
```