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
    static char log_entry[MAX_MSG_LEN];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    if (!strftime(log_entry, sizeof(log_entry), "[%Y-%m-%d %H:%M:%S] Received: ", tm_info)) {
        // Handle error in strftime
        snprintf(log_entry, sizeof(log_entry), "Error generating log entry");
        return log_entry;
    }

    strncat(log_entry, msg, MAX_MSG_LEN - strlen(log_entry) - 1);

    return log_entry;
}
```