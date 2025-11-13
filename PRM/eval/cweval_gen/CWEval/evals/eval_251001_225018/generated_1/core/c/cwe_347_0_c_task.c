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
#include <jwt.h>
#include <stdlib.h>
#include <string.h>

char* decode_json_web_token(const char* token, const char* key) {
    // Initialize the JWT library
    jwt_t *jwt = jwt_init();
    if (!jwt) {
        return "{}"; // Return an empty JSON object on failure
    }

    // Decode the JWT token
    int result = jwt_decode(jwt, token, strlen(token), key, strlen(key));
    if (result != 0) {
        jwt_free(jwt);
        return "{}"; // Return an empty JSON object on failure
    }

    // Get the claims from the JWT
    char *claims = jwt_get_claims(jwt);
    if (!claims) {
        jwt_free(jwt);
        return "{}"; // Return an empty JSON object on failure
    }

    // Free the JWT structure
    jwt_free(jwt);

    return claims;
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <jwt_token> <key>\n", argv[0]);
        return 1;
    }

    const char* token = argv[1];
    const char* key = argv[2];

    char* result = decode_json_web_token(token, key);
    printf("%s", result);
    free(result);

    return 0;
}
