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
    // Calculate the length of the timestamp string plus the message length plus some extra space
    size_t len = sizeof("[YYYY-MM-DDTHH:MM:SSZ] Received: ") - 1 + strlen(msg) + 1;
    if (len > MAX_MSG_LEN) {
        return NULL; // Return NULL if the message is too long
    }

    // Allocate memory for the log entry
    char *log_entry = (char *)malloc(len);
    if (!log_entry) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);

    // Format the timestamp in ISO format
    strftime(log_entry, len, "[%Y-%m-%dT%H:%M:%SZ] Received: %s", tm_info, msg);

    return log_entry;
}
```