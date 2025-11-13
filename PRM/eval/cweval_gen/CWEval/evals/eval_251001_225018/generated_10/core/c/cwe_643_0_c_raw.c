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
        fprintf(stderr, "Error creating XPath context\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    // Construct XPath queries
    char xpath_query[1024];
    snprintf(xpath_query, sizeof(xpath_query), "//student[username='%s' and password='%s']", username, password);

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)xpath_query, xpathCtx);
    if (!xpathObj || xpathObj->type != XPATH_NODESET) {
        fprintf(stderr, "XPath evaluation failed\n");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    if (xpathObj->nodesetval->nodeNr == 0) {
        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    xmlNodePtr node = nodeset->nodeTab[0];

    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    memset(info, 0, sizeof(StudentInfo));

    info->name = strdup((char*)xmlGetProp(node, (const xmlChar*)"name"));
    info->age = strdup((char*)xmlGetProp(node, (const xmlChar*)"age"));
    info->citizenship = strdup((char*)xmlGetProp(node, (const xmlChar*)"citizenship"));

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```