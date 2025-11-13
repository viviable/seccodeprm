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

    char query[1024];
    snprintf(query, sizeof(query), "//student[username='%s' and password='%s']", username, password);

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(BAD_CAST query, xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        fprintf(stderr, "No matching student found\n");
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodeSetPtr nodeset = xpathObj->u.nodesetval;
    if (nodeset->nodeNr == 0) {
        fprintf(stderr, "No matching student found\n");
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodePtr node = nodeset->nodeTab[0]->children;
    StudentInfo* info = malloc(sizeof(StudentInfo));
    info->name = NULL;
    info->age = NULL;
    info->citizenship = NULL;

    while (node) {
        if (!xmlStrcmp(node->name, BAD_CAST "name")) {
            info->name = strdup((const char*)xmlNodeGetContent(node));
        } else if (!xmlStrcmp(node->name, BAD_CAST "age")) {
            info->age = strdup((const char*)xmlNodeGetContent(node));
        } else if (!xmlStrcmp(node->name, BAD_CAST "citizenship")) {
            info->citizenship = strdup((const char*)xmlNodeGetContent(node));
        }
        node = node->next;
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    if (!info->name || !info->age || !info->citizenship) {
        free_student_info(info);
        return NULL;
    }

    return info;
}
```