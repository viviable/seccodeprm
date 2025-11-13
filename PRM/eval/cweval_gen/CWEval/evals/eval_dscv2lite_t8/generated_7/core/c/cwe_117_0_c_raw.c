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
    if (msg == NULL || strlen(msg) >= MAX_MSG_LEN - 10) { // Account for timestamp and 'Received: '
        return NULL;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", tm_info);

    size_t total_len = strlen(timestamp) + strlen(msg) + 14; // 14 for "[] Received: "
    char *log_entry = (char *)malloc(total_len);
    if (log_entry == NULL) {
        return NULL;
    }

    snprintf(log_entry, total_len, "[%s] Received: %s", timestamp, msg);
    return log_entry;
}
```