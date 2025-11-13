```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MSG_LEN 256

// Function to convert time_t to ISO format string
void time_to_iso(time_t t, char *buf, size_t len) {
    struct tm *tm_info = localtime(&t);
    strftime(buf, len, "%Y-%m-%dT%H:%M:%S%z", tm_info);
}

/**
Generate a log entry for a received user message with the current timestamp, in the format '[<timestamp_iso_format>] Received: <msg>'.

@param msg The user message received.

@return The log entry for the received message.
*/
char* generate_receive_log(const char *msg) {
    static char log_entry[MAX_MSG_LEN];
    time_t now = time(NULL);
    char iso_time[30];

    time_to_iso(now, iso_time, sizeof(iso_time));

    snprintf(log_entry, MAX_MSG_LEN, "[%s] Received: %s", iso_time, msg);

    return log_entry;
}
```