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
    // Calculate the length of the log entry
    size_t len = strlen(msg);
    if (len > MAX_MSG_LEN - 30) { // Reserve space for timestamp and format characters
        return NULL; // Message too long
    }

    // Allocate memory for the log entry
    char *log_entry = malloc(MAX_MSG_LEN);
    if (log_entry == NULL) {
        return NULL; // Memory allocation failed
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Format the timestamp in ISO 8601 format
    strftime(log_entry, MAX_MSG_LEN, "[%Y-%m-%dT%H:%M:%S]", tm_info);

    // Append the message to the log entry
    strcat(log_entry, " Received: ");
    strncat(log_entry + strlen(log_entry), msg, MAX_MSG_LEN - strlen(log_entry) - 1);

    return log_entry;
}
```