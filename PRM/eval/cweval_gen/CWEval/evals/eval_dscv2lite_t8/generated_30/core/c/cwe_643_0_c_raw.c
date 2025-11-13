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
    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    memset(info, 0, sizeof(StudentInfo));

    xmlChar* name = xmlGetProp(node, BAD_CAST "name");
    if (name) {
        info->name = strdup((char*)name);
        xmlFree(name);
    }

    xmlChar* age = xmlGetProp(node, BAD_CAST "age");
    if (age) {
        info->age = strdup((char*)age);
        xmlFree(age);
    }

    xmlChar* citizenship = xmlGetProp(node, BAD_CAST "citizenship");
    if (citizenship) {
        info->citizenship = strdup((char*)citizenship);
        xmlFree(citizenship);
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```