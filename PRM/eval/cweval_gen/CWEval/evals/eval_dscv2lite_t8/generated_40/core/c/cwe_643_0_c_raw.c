```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

typedef struct {
    char* name;
    char* age;
    char* citizenship;
} StudentInfo;

static void free_student_info(StudentInfo* info) {
    if (info != NULL) {
        free(info->name);
        free(info->age);
        free(info->citizenship);
        free(info);
    }
}

StudentInfo* get_student_info(const char* xml_data, const char* username, const char* password) {
    xmlDocPtr doc = xmlReadMemory(xml_data, strlen(xml_data), "noname.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse XML\n");
        return NULL;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL) {
        fprintf(stderr, "Failed to create XPath context\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    char query[1024];
    snprintf(query, sizeof(query), "//student[username='%s' and password='%s']", username, password);

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)query, xpathCtx);
    if (xpathObj == NULL) {
        fprintf(stderr, "Failed to evaluate XPath expression\n");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    StudentInfo* info = NULL;
    if (xpathObj->type == XPATH_NODESET && xpathObj->nodesetval != NULL) {
        if (xpathObj->nodesetval->nodeNr > 0) {
            xmlNodeSetPtr nodeset = xpathObj->nodesetval;
            xmlNodePtr node = nodeset->nodeTab[0];

            info = (StudentInfo*)malloc(sizeof(StudentInfo));
            memset(info, 0, sizeof(StudentInfo));

            xmlNodePtr child = node->children;
            while (child != NULL) {
                if (strcmp(child->name, "name") == 0) {
                    info->name = strdup((char*)xmlNodeGetContent(child));
                } else if (strcmp(child->name, "age") == 0) {
                    info->age = strdup((char*)xmlNodeGetContent(child));
                } else if (strcmp(child->name, "citizenship") == 0) {
                    info->citizenship = strdup((char*)xmlNodeGetContent(child));
                }
                child = child->next;
            }
        }
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```