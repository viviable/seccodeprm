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
    if (info->name) free(info->name);
    if (info->age) free(info->age);
    if (info->citizenship) free(info->citizenship);
    free(info);
}

StudentInfo* get_student_info(const char* xml_data, const char* username, const char* password) {
    xmlDocPtr doc = xmlReadMemory(xml_data, strlen(xml_data), "noname.xml", NULL, 0);
    if (!doc) {
        fprintf(stderr, "Failed to parse XML\n");
        return NULL;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        fprintf(stderr, "Failed to create XPath context\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    // Construct XPath query
    char xpathQuery[1024];
    snprintf(xpathQuery, sizeof(xpathQuery),
             "//student[username='%s' and password='%s']", username, password);

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST xpathQuery, xpathCtx);
    if (!xpathObj || !xpathObj->nodesetval || xpathObj->nodesetval->nodeNr == 0) {
        fprintf(stderr, "No matching student found\n");
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    xmlNodePtr node = nodeset->nodeTab[0];

    // Extract student information
    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    memset(info, 0, sizeof(StudentInfo));

    xmlNodePtr childNode = node->children;
    while (childNode != NULL) {
        if (strcmp((const char*)childNode->name, "name") == 0) {
            info->name = strdup((const char*)xmlNodeGetContent(childNode));
        } else if (strcmp((const char*)childNode->name, "age") == 0) {
            info->age = strdup((const char*)xmlNodeGetContent(childNode));
        } else if (strcmp((const char*)childNode->name, "citizenship") == 0) {
            info->citizenship = strdup((const char*)xmlNodeGetContent(childNode));
        }
        childNode = childNode->next;
    }

    // Cleanup
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```