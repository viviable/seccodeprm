```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO 8601 format
void time_to_iso(time_t t, char *buf, size_t len) {
    struct tm *tm_info = localtime(&t);
    if (strftime(buf, len, "%Y-%m-%dT%H:%M:%S%z", tm_info)) {
        // Adjust timezone offset from +HHMM to +HH:mm
        int tz_offset = atoi(buf + strlen(buf) - 4);
        buf[strlen(buf) - 4] = '\0';
        sprintf(buf + strlen(buf), ":%02d", abs(tz_offset / 100));
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
    char iso_time[64];

    time_to_iso(now, iso_time, sizeof(iso_time));

    snprintf(log_entry, sizeof(log_entry), "[%s] Received: %s", iso_time, msg);

    return log_entry;
}
```