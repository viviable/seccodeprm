```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to generate ISO formatted timestamp
void get_iso_timestamp(char *buffer, size_t buffer_size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, buffer_size, "%Y-%m-%dT%H:%M:%S%z", tm_info);
}

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    if (msg == NULL) {
        return NULL;
    }

    // Allocate memory for the log entry
    char *log_entry = (char *)malloc(MAX_MSG_LEN * sizeof(char));
    if (log_entry == NULL) {
        return NULL; // Memory allocation failed
    }

    // Get current timestamp in ISO format
    char timestamp[MAX_MSG_LEN];
    get_iso_timestamp(timestamp, MAX_MSG_LEN);

    // Format the log entry
    snprintf(log_entry, MAX_MSG_LEN, "[%s] Received: %s", timestamp, msg);

    return log_entry;
}
```