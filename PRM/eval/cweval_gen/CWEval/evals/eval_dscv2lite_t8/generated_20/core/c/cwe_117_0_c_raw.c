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
    // Allocate memory for the log entry
    char *log_entry = (char *)malloc(MAX_MSG_LEN * sizeof(char));
    if (log_entry == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Get the current time
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Format the timestamp as ISO format
    strftime(log_entry, MAX_MSG_LEN, "[%Y-%m-%dT%H:%M:%S%z] ", tm_info);

    // Append the message to the log entry
    snprintf(log_entry + strlen(log_entry), MAX_MSG_LEN - strlen(log_entry), "Received: %s", msg);

    return log_entry;
}
```