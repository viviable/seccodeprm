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
    if (msg == NULL || strlen(msg) >= MAX_MSG_LEN - 18) {
        // Handle invalid input
        return NULL;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char *iso_timestamp = malloc(20); // YYYY-MM-DDTHH:MM:SS
    strftime(iso_timestamp, 20, "%Y-%m-%dT%H:%M:%S", tm_info);

    char *log_entry = malloc(MAX_MSG_LEN);
    snprintf(log_entry, MAX_MSG_LEN, "[%s] Received: %s", iso_timestamp, msg);

    free(iso_timestamp);
    return log_entry;
}
```