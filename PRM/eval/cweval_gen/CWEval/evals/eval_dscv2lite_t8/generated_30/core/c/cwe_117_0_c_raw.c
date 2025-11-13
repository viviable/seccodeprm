```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO 8601 format
void time_to_iso(char *buf, size_t buf_size, time_t t) {
    struct tm *tm_info = localtime(&t);
    if (strftime(buf, buf_size, "%Y-%m-%dT%H:%M:%S%z", tm_info)) {
        // Adjust timezone offset to match ISO 8601 format
        int tz_offset = tm_info->tm_gmtoff / 3600;
        snprintf(buf + strlen(buf) - 2, 4, "%+02d:00", tz_offset);
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

    time_to_iso(timestamp, sizeof(timestamp), now);

    snprintf(log_entry, sizeof(log_entry), "[%s] Received: %s", timestamp, msg);

    return log_entry;
}
```