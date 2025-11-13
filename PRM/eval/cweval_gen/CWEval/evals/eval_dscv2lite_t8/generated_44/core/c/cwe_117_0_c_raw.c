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

    if (strftime(log_entry, sizeof(log_entry), "[%Y-%m-%d %H:%M:%S] Received: %s", tm_info, msg) == 0) {
        // Handle error, e.g., set an empty string or log it
        log_entry[0] = '\0';
    }

    return log_entry;
}
```