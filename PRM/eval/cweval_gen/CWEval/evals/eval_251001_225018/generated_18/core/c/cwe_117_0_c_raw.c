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
    if (msg == NULL) {
        return NULL;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S%z", tm_info);

    int total_len = snprintf(NULL, 0, "[%s] Received: %s", timestamp, msg) + 1;
    if (total_len > MAX_MSG_LEN) {
        return NULL; // Log entry too long
    }

    char *log_entry = (char *)malloc(total_len);
    if (log_entry == NULL) {
        return NULL; // Memory allocation failed
    }

    snprintf(log_entry, total_len, "[%s] Received: %s", timestamp, msg);

    return log_entry;
}
```