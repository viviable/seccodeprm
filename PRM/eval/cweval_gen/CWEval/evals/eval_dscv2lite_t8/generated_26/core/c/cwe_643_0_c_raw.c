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
        fprintf(stderr, "Failed to parse XML\n");
        return NULL;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        fprintf(stderr, "Error creating XPath context\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    char query[256];
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

    xmlNodePtr node = nodeset->nodeTab[0];
    xmlChar* name = xmlGetProp(node, BAD_CAST "name");
    xmlChar* age = xmlGetProp(node, BAD_CAST "age");
    xmlChar* citizenship = xmlGetProp(node, BAD_CAST "citizenship");

    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    info->name = strdup((const char*)name);
    info->age = strdup((const char*)age);
    info->citizenship = strdup((const char*)citizenship);

    xmlFree(name);
    xmlFree(age);
    xmlFree(citizenship);
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```