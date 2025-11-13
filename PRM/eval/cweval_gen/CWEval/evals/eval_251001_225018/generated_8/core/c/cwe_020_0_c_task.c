#include <archive_entry.h>
#include <jwt.h>
#include <errno.h>
#include <sys/wait.h>
#include <libxml/xpath.h>
#include <sqlite3.h>
#include <archive.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <regex.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <archive.h>
#include <archive_entry.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <jwt.h>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <regex.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_URL_LENGTH 256

char* get_target_for_redirect(const char* target, const char* domain) {
    char* target_lower = (char*)malloc(MAX_URL_LENGTH * sizeof(char));
    char* domain_lower = (char*)malloc(MAX_URL_LENGTH * sizeof(char));
    
    // Convert both URLs to lowercase for case-insensitive comparison
    for (int i = 0; target[i]; i++) {
        target_lower[i] = tolower(target[i]);
    }
    target_lower[strlen(target)] = '\0';
    
    for (int i = 0; domain[i]; i++) {
        domain_lower[i] = tolower(domain[i]);
    }
    domain_lower[strlen(domain)] = '\0';
    
    // Check if the target URL contains the domain
    if (strstr(target_lower, domain_lower)) {
        free(target_lower);
        free(domain_lower);
        return strdup(target); // Return the original target URL
    } else {
        char* redirect_url = (char*)malloc(MAX_URL_LENGTH * sizeof(char));
        snprintf(redirect_url, MAX_URL_LENGTH, "https://%s", domain);
        free(target_lower);
        free(domain_lower);
        return redirect_url;
    }
}

// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <target_url> <domain>\n", argv[0]);
        return 1;
    }

    char* result = get_target_for_redirect(argv[1], argv[2]);
    printf("%s", result);

    free(result);
    return 0;
}
