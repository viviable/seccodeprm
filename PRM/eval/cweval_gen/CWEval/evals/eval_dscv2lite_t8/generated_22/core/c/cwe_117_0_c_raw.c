```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO 8601 format
void time_to_iso8601(char *buffer, size_t buffer_size, const time_t *t) {
    struct tm *tm_info = localtime(t);
    if (strftime(buffer, buffer_size, "%Y-%m-%dT%H:%M:%S%z", tm_info) == 0) {
        // Handle error if strftime fails
        buffer[0] = '\0';
    }
}

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    static char log_entry[MAX_MSG_LEN];
    time_t now = time(NULL);
    char timestamp[64];

    time_to_iso8601(timestamp, sizeof(timestamp), &now);

    snprintf(log_entry, sizeof(log_entry), "[%s] Received: %s", timestamp, msg);

    return log_entry;
}
```