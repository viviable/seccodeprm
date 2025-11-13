```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO format string
void time_to_iso(char *buffer, size_t buffer_size, const time_t *time_ptr) {
    struct tm *tm_info = localtime(time_ptr);
    strftime(buffer, buffer_size, "%Y-%m-%dT%H:%M:%S%z", tm_info);
}

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    // Allocate memory for the log entry
    char *log_entry = (char *)malloc(MAX_MSG_LEN + strlen(msg));
    if (log_entry == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Get the current time
    time_t now = time(NULL);

    // Convert time to ISO format
    char iso_time[MAX_MSG_LEN];
    time_to_iso(iso_time, sizeof(iso_time), &now);

    // Format the log entry
    snprintf(log_entry, MAX_MSG_LEN + strlen(msg), "[%s] Received: %s", iso_time, msg);

    return log_entry;
}
```