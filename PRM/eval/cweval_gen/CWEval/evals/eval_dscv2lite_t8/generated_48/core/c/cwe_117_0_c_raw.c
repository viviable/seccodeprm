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
    size_t len = strlen(msg) + sizeof("[YYYY-MM-DDTHH:MM:SSZ] Received: ") - 1;
    if (len >= MAX_MSG_LEN) {
        return NULL; // Message too long
    }

    // Allocate memory for the log entry
    char *log_entry = malloc(len + 1);
    if (!log_entry) {
        return NULL; // Memory allocation failed
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);

    // Format the timestamp
    strftime(log_entry, len, "[%.4d-%.2d-%.2dT%.2d:%.2d:%.2dZ] Received: ", tm_info);

    // Append the message to the log entry
    strncat(log_entry, msg, len - strlen(log_entry));

    return log_entry;
}
```