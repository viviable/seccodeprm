```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO 8601 format string
void time_to_iso8601(char *buffer, size_t buffer_size, const struct tm *tm) {
    strftime(buffer, buffer_size, "%Y-%m-%dT%H:%M:%S", tm);
}

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    static char log_entry[MAX_MSG_LEN];
    time_t now = time(NULL);
    struct tm tm_struct;
    localtime_r(&now, &tm_struct);

    char iso_time[64];
    time_to_iso8601(iso_time, sizeof(iso_time), &tm_struct);

    snprintf(log_entry, MAX_MSG_LEN, "[%s] Received: %s", iso_time, msg);
    return log_entry;
}
```