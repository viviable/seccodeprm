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
    if (msg == NULL || strlen(msg) >= MAX_MSG_LEN - 10) { // Ensure there's space for timestamp and format characters
        return NULL;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char *log_entry = (char *)malloc(MAX_MSG_LEN);
    if (log_entry == NULL) {
        return NULL; // Memory allocation failed
    }

    strftime(log_entry, MAX_MSG_LEN, "[%Y-%m-%d %H:%M:%S] Received: %s", tm_info, msg);
    return log_entry;
}
```