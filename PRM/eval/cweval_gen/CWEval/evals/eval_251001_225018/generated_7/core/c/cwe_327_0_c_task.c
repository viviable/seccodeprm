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
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>

/**
Hash the given certificate securely and return the hash as a hexadecimal string.
You can include openssh libraries.

@param cert The certificate to be hashed.

@return The securely hashed certificate as a hexadecimal string.
*/
char* hash_certificate(const char *cert) {
    // Define the SHA-256 hash size
    const int sha256_size = SHA256_DIGEST_LENGTH;
    
    // Allocate memory for the SHA-256 hash
    unsigned char *sha256_hash = malloc(sha256_size);
    if (!sha256_hash) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    // Compute the SHA-256 hash of the certificate
    SHA256_CTX sha256_ctx;
    if (!SHA256_Init(&sha256_ctx)) {
        fprintf(stderr, "Failed to initialize SHA256 context\n");
        free(sha256_hash);
        return NULL;
    }
    if (!SHA256_Update(&sha256_ctx, cert, strlen(cert))) {
        fprintf(stderr, "Failed to update SHA256 with certificate data\n");
        free(sha256_hash);
        return NULL;
    }
    if (!SHA256_Final(sha256_hash, &sha256_ctx)) {
        fprintf(stderr, "Failed to finalize SHA256 computation\n");
        free(sha256_hash);
        return NULL;
    }
    
    // Allocate memory for the hexadecimal representation of the hash
    char *hex_string = malloc(2 * sha256_size + 1);
    if (!hex_string) {
        fprintf(stderr, "Memory allocation failed\n");
        free(sha256_hash);
        return NULL;
    }
    
    // Convert the binary hash to a hexadecimal string
    for (int i = 0; i < sha256_size; ++i) {
        sprintf(hex_string + 2 * i, "%02x", sha256_hash[i]);
    }
    
    // Free the allocated memory for the hash
    free(sha256_hash);
    
    return hex_string;
}

// BEGIN ENTRYPOINT
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <certificate>\n", argv[0]);
        return 1;
    }

    char *hashed_cert = hash_certificate(argv[1]);
    if (hashed_cert) {
        printf("%s", hashed_cert);
        free(hashed_cert);
    }

    return 0;
}
