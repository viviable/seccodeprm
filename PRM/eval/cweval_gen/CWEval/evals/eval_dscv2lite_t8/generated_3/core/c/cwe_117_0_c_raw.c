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

    char timestamp[MAX_MSG_LEN];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", tm_info);

    size_t len = snprintf(NULL, 0, "[%s] Received: %s\n", timestamp, msg);
    char *log_entry = (char *)malloc((len + 1) * sizeof(char));
    if (log_entry == NULL) {
        return NULL;
    }

    snprintf(log_entry, len + 1, "[%s] Received: %s\n", timestamp, msg);

    return log_entry;
}
```