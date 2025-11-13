```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO format string
void time_to_iso(char *buffer, size_t buffer_size, time_t t) {
    struct tm *tm_info = gmtime(&t);
    if (strftime(buffer, buffer_size, "%Y-%m-%dT%H:%M:%SZ", tm_info) == 0) {
        // Handle error
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
    char iso_time[20]; // Assuming ISO format will not exceed 19 characters + null terminator

    time_to_iso(iso_time, sizeof(iso_time), now);

    snprintf(log_entry, sizeof(log_entry), "[%s] Received: %s", iso_time, msg);

    return log_entry;
}
```