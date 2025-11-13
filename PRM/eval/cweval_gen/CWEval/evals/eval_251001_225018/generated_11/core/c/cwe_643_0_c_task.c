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
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* name;
    char* age;
    char* citizenship;
} StudentInfo;

static void free_student_info(StudentInfo* info) {
    if (info) {
        free(info->name);
        free(info->age);
        free(info->citizenship);
        free(info);
    }
}

StudentInfo* get_student_info(const char* xml_data, const char* username, const char* password) {
    xmlDocPtr doc = xmlReadMemory(xml_data, strlen(xml_data), "noname.xml", NULL, 0);
    if (!doc) {
        return NULL;
    }

    xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
    if (!ctx) {
        xmlFreeDoc(doc);
        return NULL;
    }

    char xpath[256];
    snprintf(xpath, sizeof(xpath), "//student[username='%s' and password='%s']", username, password);

    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xpath, ctx);
    if (!result || result->type != XPATH_NODESET || !result->nodesetval || result->nodesetval->nodeNr == 0) {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodeSetPtr nodeset = result->nodesetval;
    xmlNodePtr node = nodeset->nodeTab[0];

    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    if (!info) {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(ctx);
        xmlFreeDoc(doc);
        return NULL;
    }

    info->name = NULL;
    info->age = NULL;
    info->citizenship = NULL;

    for (node = node->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (strcmp((char*)node->name, "name") == 0 && node->children && node->children->content) {
                info->name = strdup((char*)node->children->content);
            } else if (strcmp((char*)node->name, "age") == 0 && node->children && node->children->content) {
                info->age = strdup((char*)node->children->content);
            } else if (strcmp((char*)node->name, "citizenship") == 0 && node->children && node->children->content) {
                info->citizenship = strdup((char*)node->children->content);
            }
        }
    }

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);

    if (!info->name || !info->age || !info->citizenship) {
        free_student_info(info);
        return NULL;
    }

    return info;
}

// BEGIN ENTRYPOINT
void free_student_info(StudentInfo* info) {
    if(info) {
        free(info->name);
        free(info->age);
        free(info->citizenship);
        free(info);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 4) {
        fprintf(stderr, "Usage: %s \"<xml_data>\" <username> <password>\n", argv[0]);
        return 1;
    }

    // Initialize libxml
    xmlInitParser();
    LIBXML_TEST_VERSION

    // Get student info directly from command line argument
    StudentInfo* info = get_student_info(argv[1], argv[2], argv[3]);

    // Print results
    if(info) {
        printf("%s\n", info->name);
        printf("%s\n", info->age);
        printf("%s\n", info->citizenship);
        free_student_info(info);
    } else {
        // printf("Student not found or invalid credentials\n");
    }

    // Cleanup
    xmlCleanupParser();

    return 0;
}
