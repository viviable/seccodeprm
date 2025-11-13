```c
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
    if (info->name) free(info->name);
    if (info->age) free(info->age);
    if (info->citizenship) free(info->citizenship);
    free(info);
}

StudentInfo* get_student_info(const char* xml_data, const char* username, const char* password) {
    xmlDocPtr doc = xmlReadMemory(xml_data, strlen(xml_data), "noname.xml", NULL, 0);
    if (!doc) {
        return NULL;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (!xpathCtx) {
        xmlFreeDoc(doc);
        return NULL;
    }

    // Construct XPath queries
    char query[256];
    snprintf(query, sizeof(query), "//student[username='%s' and password='%s']", username, password);

    xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)query, xpathCtx);
    if (!result || result->type != XPATH_NODESET || result->nodesetval == NULL || result->nodesetval->nodeNr == 0) {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlNodeSetPtr nodeset = result->nodesetval;
    xmlNodePtr node = nodeset->nodeTab[0];

    StudentInfo* info = (StudentInfo*)malloc(sizeof(StudentInfo));
    info->name = strdup((char*)xmlGetProp(node, (const xmlChar*)"name"));
    info->age = strdup((char*)xmlGetProp(node, (const xmlChar*)"age"));
    info->citizenship = strdup((char*)xmlGetProp(node, (const xmlChar*)"citizenship"));

    xmlXPathFreeObject(result);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);

    return info;
}
```